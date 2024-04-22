

#include "SGPawnManagerComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Net/UnrealNetwork.h"
#include "SurvivalGame/SGGameplayTags.h"
#include "SurvivalGame/AbilitySystem/SGAbilitySystemComponent.h"
#include "SurvivalGame/Input/SGInputComponent.h"
#include "SurvivalGame/Player/SGPlayerState.h"

const FName USGPawnManagerComponent::NAME_ActorFeatureName("PawnManager");
const FName USGPawnManagerComponent::NAME_BindInputsReady("BindInputsReady");

USGPawnManagerComponent::USGPawnManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}

void USGPawnManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterData);
}

void USGPawnManagerComponent::OnRep_CharacterData()
{
	CheckDefaultInitialization();
}

void USGPawnManagerComponent::OnRegister()
{
	Super::OnRegister();

	RegisterInitStateFeature();
}

void USGPawnManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ensure(TryToChangeInitState(SGGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void USGPawnManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void USGPawnManagerComponent::UpdateInitializationState()
{
	CheckDefaultInitialization();
}

void USGPawnManagerComponent::SetCharacterData(const USGCharacterConfig* InCharacterData)
{
	check(InCharacterData);

	APawn* OwningPawn = GetPawnChecked<APawn>();
	if (OwningPawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	CharacterData = InCharacterData;
	OwningPawn->ForceNetUpdate();
	CheckDefaultInitialization();
}

void USGPawnManagerComponent::CheckDefaultInitialization()
{
	// try progress other game feature dependencies
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {
		SGGameplayTags::InitState_Spawned,
		SGGameplayTags::InitState_DataAvailable,
		SGGameplayTags::InitState_DataInitialized,
		SGGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

bool USGPawnManagerComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* OwningPawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == SGGameplayTags::InitState_Spawned)
	{
		if (OwningPawn)
		{
			return true;
		}
	}
	else if (CurrentState == SGGameplayTags::InitState_Spawned && DesiredState == SGGameplayTags::InitState_DataAvailable)
	{
		if (CharacterData == nullptr)
		{
			return false;
		}

		if (OwningPawn->IsLocallyControlled() || OwningPawn->HasAuthority())
		{
			const APlayerController* PC = GetController<APlayerController>();
			if (!OwningPawn->InputComponent || !PC || !PC->GetLocalPlayer())
			{
				return false;
			}
		}
		
		return true;
	}
	else if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(OwningPawn, SGGameplayTags::InitState_DataAvailable);
	}
	else if (CurrentState == SGGameplayTags::InitState_DataInitialized && DesiredState == SGGameplayTags::InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
}

void USGPawnManagerComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	
}

void USGPawnManagerComponent::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	check(InAbilitySystemComponent);
	AbilitySystemComponent = Cast<USGAbilitySystemComponent>(InAbilitySystemComponent);
}

void USGPawnManagerComponent::BindAdditionalInputConfig(const USGInputConfig* InputConfig) const
{
	OnBindAdditionalInputConfig.Broadcast(InputConfig);
}

void USGPawnManagerComponent::ListenOnAdditionalInputConfig(FOnBindAdditionalInputConfig::FDelegate&& Delegate)
{
	if (!OnBindAdditionalInputConfig.IsBoundToObject(Delegate.GetUObject()))
	{
		OnBindAdditionalInputConfig.Add(Delegate);
	}
}

void USGPawnManagerComponent::RemoveInputBindings() const
{
	OnRemoveInputBindings.Broadcast();
}

void USGPawnManagerComponent::ListenOnRemoveInputBindings(FOnRemoveInputBindings::FDelegate&& Delegate)
{
	if (!OnRemoveInputBindings.IsBoundToObject(Delegate.GetUObject()))
	{
		OnRemoveInputBindings.Add(Delegate);
	}
}
