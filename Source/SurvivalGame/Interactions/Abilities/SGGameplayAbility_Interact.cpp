// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "SurvivalGame/Interactions/InteractionStatics.h"
#include "SurvivalGame/Interactions/InteractOption.h"
#include "SurvivalGame/UI/Interaction/InteractionDescriptor.h"
#include "SurvivalGame/UI/Interaction/InteractionManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SGGameplayAbility_Interact)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Interaction_Activate, "Ability.Interaction.Activate");

USGGameplayAbility_Interact::USGGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActivateAbilityOnGranted = true;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USGGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USGGameplayAbility_Interact::ApplyInteractOptionActivationRequirement(const FInteractionDefinition& InteractionDefinition)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC == nullptr)
	{
		return;
	}
	
	const AActor* AvatarActor = ASC->GetAvatarActor();
	if (AvatarActor == nullptr)
	{
		return;
	}

	
	if (AActor* InteractableActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionDefinition.InteractableTarget))
	{
		TArray<FGameplayTag> GrantedTags;
		GetInteractionGrantedTags(InteractionDefinition, GrantedTags);

		if (GrantedTags.Num() > 0)
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddActors({ InteractableActor });
			EffectContextHandle.AddSourceObject(AvatarActor);

			FHitResult HitResult;
			HitResult.HitObjectHandle = InteractableActor;
			EffectContextHandle.AddHitResult(HitResult);

			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(
				DynamicInteractionGameplayEffectClass, 1, EffectContextHandle);

			FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
			for (const FGameplayTag& GrantedTag : GrantedTags)
			{
				Spec->DynamicGrantedTags.AddTag(GrantedTag);
			}
			
			ActiveGameplayEffectHandles.Add(ASC->ApplyGameplayEffectSpecToSelf(*Spec));
		}
	}
}

void USGGameplayAbility_Interact::ClearInteractOptionActivationRequirement()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC == nullptr)
	{
		return;
	}

	for (const FActiveGameplayEffectHandle& EffectHandle : ActiveGameplayEffectHandles)
	{
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}

	ActiveGameplayEffectHandles.Empty();
}

void USGGameplayAbility_Interact::ShowInteractionPanel(const FInteractionDefinition& InteractionDefinition)
{
	if (const APlayerController* PC = GetPlayerControllerFromActorInfo())
	{
		if (UInteractionManagerComponent* InteractionManager = PC->FindComponentByClass<UInteractionManagerComponent>())
		{
			const TSoftClassPtr<UUserWidget> InteractionWidgetClass = !InteractionDefinition.InteractWidgetClass.IsNull()
				? InteractionDefinition.InteractWidgetClass
				: DefaultInteractionWidgetClass;

			const FVector WorldLocation = UInteractionStatics::GetInteractableActorWorldLocation(
				InteractionDefinition.InteractableTarget);

			InteractionDescriptor = NewObject<UInteractionDescriptor>();
			InteractionDescriptor->SetInteractableObjectName(InteractionDefinition.Name);;
			InteractionDescriptor->SetInteractionWidgetClass(InteractionWidgetClass);
			InteractionDescriptor->SetInteractableTarget(InteractionDefinition.InteractableTarget);
			InteractionDescriptor->SetInteractionOption(InteractionDefinition.InteractOptions);
			InteractionDescriptor->SetWorldLocation(WorldLocation);

			InteractionManager->ShowInteraction(InteractionDescriptor);
		}
	}
}

void USGGameplayAbility_Interact::HideInteractionPanel()
{
	if (const APlayerController* PC = GetPlayerControllerFromActorInfo())
	{
		if (UInteractionManagerComponent* InteractionManager = PC->FindComponentByClass<UInteractionManagerComponent>())
		{
			InteractionManager->HideInteraction();
			InteractionDescriptor = nullptr;
		}
	}
}

void USGGameplayAbility_Interact::GetInteractionGrantedTags(const FInteractionDefinition& InteractionDefinition, TArray<FGameplayTag>& OutGrantedTags) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC == nullptr)
	{
		return;
	}
	
	for (const FInteractOption& InteractOption : InteractionDefinition.InteractOptions)
	{
		if (const FGameplayAbilitySpec* AbilitySpecHandle = ASC->FindAbilitySpecFromClass(InteractOption.InteractionAbilityToGrant))
		{
			if (const USGGameplayAbility_InteractAction* InteractAbility = Cast<USGGameplayAbility_InteractAction>(AbilitySpecHandle->Ability))
			{
				if (InteractAbility->InteractActiveGrantedTag.IsValid())
				{
					OutGrantedTags.Add(InteractAbility->InteractActiveGrantedTag);
				}
			}
		}
	}
}
