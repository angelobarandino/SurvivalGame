// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddAbilities.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include "AbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "SurvivalGame/AbilitySystem/SGAbilitySystemComponent.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SurvivalGame/Player/SGPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddAbilities)

#define LOCTEXT_NAMESPACE "GameFeatures"

void UGameFeatureAction_AddAbilities::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ActiveExtensions.IsEmpty()) ||
		!ensure(ActiveData.ComponentRequests.IsEmpty()))
	{
		Reset(ActiveData);
	}
	
	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddAbilities::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddAbilities::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (int Index = 0; Index < AbilitiesList.Num(); ++Index)
	{
		const FGameFeatureAbilitiesEntry& Entry = AbilitiesList[Index];
		if (Entry.ActorClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(Index)));
		}

		if (Entry.GrantedAbilities.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNoAddOns", "Index {0} in AbilitiesList will do nothing (no granted abilities, attributes, or ability sets)"), FText::AsNumber(Index)));
		}

		int32 AbilityIndex = 0;
		for (const TSoftClassPtr<UGameplayAbility>& Ability : Entry.GrantedAbilities)
		{
			if (Ability.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullAbility", "Null AbilityType at index {0} in AbilitiesList[{1}].GrantedAbilities"), FText::AsNumber(AbilityIndex), FText::AsNumber(Index)));
			}
			++AbilityIndex;
		}
	}
	
	return Result;
}
#endif

void UGameFeatureAction_AddAbilities::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	const UWorld* World = WorldContext.World();
	const UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((GameInstance != nullptr) && (World != nullptr) && (World->IsGameWorld()))
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			for (int EntryIndex = 0; EntryIndex < AbilitiesList.Num(); ++EntryIndex)
			{
				const FGameFeatureAbilitiesEntry& Entry = AbilitiesList[EntryIndex];
				const UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
					this, &ThisClass::HandleActorExtension, EntryIndex, ChangeContext);

				const TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
					ComponentManager->AddExtensionHandler(Entry.ActorClass, AddAbilitiesDelegate);

				ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
			}
		}
	}
}

void UGameFeatureAction_AddAbilities::Reset(FPerContextData& ActiveData) const
{
	while (!ActiveData.ActiveExtensions.IsEmpty())
	{
		const auto ExtensionIt = ActiveData.ActiveExtensions.CreateIterator();
		RemoveActorAbilities(ExtensionIt->Key, ActiveData);
	}

	ActiveData.ComponentRequests.Empty();
}

void UGameFeatureAction_AddAbilities::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData* ActiveData = ContextData.Find(ChangeContext);
	if (AbilitiesList.IsValidIndex(EntryIndex) && ActiveData)
	{
		const FGameFeatureAbilitiesEntry& Entry = AbilitiesList[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
		{
			RemoveActorAbilities(Actor, *ActiveData);
		}
		else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == ASGPlayerState::NAME_AbilitiesReady)
		{
			AddActorAbilities(Actor, Entry, *ActiveData);
		}
	}
}

void UGameFeatureAction_AddAbilities::AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilityEntry, FPerContextData& ActiveData)
{
	check(Actor);
	if (!Actor->HasAuthority())
	{
		return;
	}

	if (ActiveData.ActiveExtensions.Find(Actor) != nullptr)
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = FindOrAddComponentForActor<UAbilitySystemComponent>(Actor, AbilityEntry, ActiveData))
	{
		FActorExtensions AddedExtensions;
		AddedExtensions.Abilities.Reserve(AbilityEntry.GrantedAbilities.Num());

		for (const TSoftClassPtr<UGameplayAbility>& Ability : AbilityEntry.GrantedAbilities)
		{
			FGameplayAbilitySpec NewAbilitySpec(Ability.LoadSynchronous());
			FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(NewAbilitySpec);
			AddedExtensions.Abilities.Add(AbilitySpecHandle);
		}

		ActiveData.ActiveExtensions.Add(Actor, AddedExtensions);
	}
}

void UGameFeatureAction_AddAbilities::RemoveActorAbilities(const AActor* Actor, FPerContextData& ActiveData) const
{
	if (FActorExtensions* ActorExtensions = ActiveData.ActiveExtensions.Find(Actor))
	{
		if (USGAbilitySystemComponent* ASC = Actor->FindComponentByClass<USGAbilitySystemComponent>())
		{
			for (const FGameplayAbilitySpecHandle AbilityHandle : ActorExtensions->Abilities)
			{
				ASC->SetRemoveAbilityOnEnd(AbilityHandle);
				
				if (const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(AbilityHandle))
				{
					if (AbilitySpec->IsActive())
					{
						ASC->CallServerEndAbility(AbilityHandle, AbilitySpec->ActivationInfo, ASC->ScopedPredictionKey);
					}
					
					// USGGameplayAbility* GSAbility = Cast<USGGameplayAbility>(AbilitySpec->Ability);
					// if (AbilitySpec->IsActive() && GSAbility != nullptr)
					// {
					// 	GSAbility->ExternalEndAbility();
					// }
				}
			}
		}

		ActiveData.ActiveExtensions.Remove(Actor);
	}
}

UActorComponent* UGameFeatureAction_AddAbilities::FindOrAddComponentForActor(UClass* ComponentType, const AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData) const
{
	UActorComponent* Component = Actor->FindComponentByClass(ComponentType);
	bool bMakeComponentRequest = (Component == nullptr);
	if (Component)
	{
		// Check to see if this component was created from a different `UGameFrameworkComponentManager` request.
		// `Native` is what `CreationMethod` defaults to for dynamically added components.
		if (Component->CreationMethod == EComponentCreationMethod::Native)
		{
			// Attempt to tell the difference between a true native component and one created by the GameFrameworkComponent system.
			// If it is from the UGameFrameworkComponentManager, then we need to make another request (requests are ref counted).
			const UObject* ComponentArchetype = Component->GetArchetype();
			bMakeComponentRequest = ComponentArchetype->HasAnyFlags(RF_ClassDefaultObject);
		}
	}
	
	if (bMakeComponentRequest)
	{
		const UWorld* World = Actor->GetWorld();
		const UGameInstance* GameInstance = World->GetGameInstance();

		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			const TSharedPtr<FComponentRequestHandle> RequestHandle = ComponentMan->AddComponentRequest(AbilitiesEntry.ActorClass, ComponentType);
			ActiveData.ComponentRequests.Add(RequestHandle);
		}

		if (!Component)
		{
			Component = Actor->FindComponentByClass(ComponentType);
			ensureAlways(Component);
		}
	}

	return Component;
}
#undef LOCTEXT_NAMESPACE
