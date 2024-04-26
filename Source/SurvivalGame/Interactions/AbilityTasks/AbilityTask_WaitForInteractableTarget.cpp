#include "AbilityTask_WaitForInteractableTarget.h"

#include "AbilitySystemComponent.h"
#include "SurvivalGame/AbilitySystem/SGAbilitySystemComponent.h"
#include "SurvivalGame/Interactions/InteractableTarget.h"
#include "SurvivalGame/Interactions/Abilities/SGGameplayAbility_InteractAction.h"
#include "SurvivalGame/Interactions/Components/InteractableScannerComponent.h"

UAbilityTask_WaitForInteractableTarget* UAbilityTask_WaitForInteractableTarget::WaitForInteractableTarget(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate, bool bShowDebug)
{
	UAbilityTask_WaitForInteractableTarget* AbilityTask = NewAbilityTask<UAbilityTask_WaitForInteractableTarget>(OwningAbility);
	AbilityTask->InteractionScanRadius = InteractionScanRange;
	AbilityTask->InteractionScanRate = InteractionScanRate;
	AbilityTask->bShowDebug = bShowDebug;
	return AbilityTask;
}

void UAbilityTask_WaitForInteractableTarget::Activate()
{
	Super::Activate();

	check(AbilitySystemComponent.Get());

	if (USGAbilitySystemComponent* ASC = Cast<USGAbilitySystemComponent>(AbilitySystemComponent.Get()))
	{
		ASC->CallOrAddOnAvatarActorSet(FOnAvatarActorSetDelegate::FDelegate::CreateUObject(this, &ThisClass::OnPawnAvatarInitialized));
	}
}

void UAbilityTask_WaitForInteractableTarget::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitForInteractableTarget::OnPawnAvatarInitialized()
{
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
	if (UInteractableScannerComponent* InteractableScanner = AvatarActor->FindComponentByClass<UInteractableScannerComponent>())
	{
		InteractableScanner->InteractionScanRadius = InteractionScanRadius;
		InteractableScanner->InteractionScanRate = InteractionScanRate;
		InteractableScanner->bShowDebug = bShowDebug;
		
		InteractableScanner->FoundInteractable.AddUObject(this, &ThisClass::OnFoundInteractableTarget);
		InteractableScanner->LostInteractable.AddUObject(this, &ThisClass::OnLostInteractableTarget);
	}
}

void UAbilityTask_WaitForInteractableTarget::OnFoundInteractableTarget(const TScriptInterface<IInteractableTarget> InteractableTarget)
{
	FInteractionDefinition InteractionDefinition;
	InteractableTarget->GetInteractionDefinition(InteractionDefinition);
	
	GrantInteractionOptionsAbility(InteractionDefinition.InteractOptions);
	FoundInteractable.Broadcast(InteractionDefinition);
}

void UAbilityTask_WaitForInteractableTarget::OnLostInteractableTarget(const TScriptInterface<IInteractableTarget> InteractableTarget)
{
	LostInteractable.Broadcast({});
}

void UAbilityTask_WaitForInteractableTarget::GrantInteractionOptionsAbility(const TArray<FInteractOption>& InteractOptions)
{
	if (GetAvatarActor()->HasAuthority())
	{
		for (const FInteractOption& InteractOption : InteractOptions)
		{
			if (InteractOption.InteractionAbilityToGrant)
			{
				FObjectKey ObjectKey(InteractOption.InteractionAbilityToGrant);
				if (InteractOptionsAbilityHandles.Find(ObjectKey))
				{
					continue;
				}

				const TSubclassOf<UGameplayAbility> AbilityClass(InteractOption.InteractionAbilityToGrant);
				FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
				FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
				InteractOptionsAbilityHandles.Add(ObjectKey, AbilitySpecHandle);
			}
		}
	}
}
