// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility_InteractAction.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "SurvivalGame/Interactions/InteractionStatics.h"
#include "SurvivalGame/UI/Interaction/InteractionLibrary.h"
#include "SurvivalGame/UI/Interaction/InteractionManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SGGameplayAbility_InteractAction)


/**
 * ///////////////////////////////////////////////////////////////////////////////////////////////////////
 * USGGameplayAbility_InteractAction
 */
USGGameplayAbility_InteractAction::USGGameplayAbility_InteractAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

AActor* USGGameplayAbility_InteractAction::GetActorFromInteractableTarget() const
{
	return UInteractionStatics::GetActorFromInteractableTarget(GetCurrentInteractableTarget());
}

TScriptInterface<IInteractableTarget> USGGameplayAbility_InteractAction::GetCurrentInteractableTarget() const
{
	if (const APawn* Pawn = Cast<APawn>(GetAvatarActorFromActorInfo()))
	{
		return UInteractionStatics::GetActiveInteractableTarget(Pawn);
	}

	return TScriptInterface<IInteractableTarget>();
}


/**
 * ///////////////////////////////////////////////////////////////////////////////////////////////////////
 * USGGameplayAbility_InteractDuration
 */
USGGameplayAbility_InteractDuration::USGGameplayAbility_InteractDuration(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractDuration = 1.f;
	WaitHoldThreshold = 0.2f;
}

void USGGameplayAbility_InteractDuration::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputRelease);
	WaitInputReleaseTask->ReadyForActivation();
	
	WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, WaitHoldThreshold);
	WaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::BeginHoldToInteract);
	WaitDelayTask->ReadyForActivation();
}

void USGGameplayAbility_InteractDuration::OnInputRelease(float TimeHeld)
{
	OnHoldToInteractCancelled();
	
	EndHoldInteract();

	if (WaitDelayTask)
	{
		WaitDelayTask->EndTask();
	}

	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USGGameplayAbility_InteractDuration::EndHoldInteract() const
{
	if (APlayerController* PC = GetPlayerControllerFromActorInfo())
	{
		if (UInteractionManagerComponent* InteractionManager = UInteractionLibrary::GetInteractionManager(PC))
		{
			InteractionManager->OnHoldProgress.RemoveDynamic(this, &ThisClass::OnHoldToInteractProgress);
			InteractionManager->OnHoldCompleted.RemoveDynamic(this, &ThisClass::OnHoldToInteractCompleted);
			InteractionManager->EndHoldInteract();
		}
	}
}

void USGGameplayAbility_InteractDuration::BeginHoldToInteract()
{
	if (APlayerController* PC = GetPlayerControllerFromActorInfo())
	{
		if (UInteractionManagerComponent* InteractionManager = UInteractionLibrary::GetInteractionManager(PC))
		{
			InteractionManager->OnHoldProgress.AddDynamic(this, &ThisClass::OnHoldToInteractProgress);
			InteractionManager->OnHoldCompleted.AddDynamic(this, &ThisClass::OnHoldToInteractCompleted);
			InteractionManager->StartHoldInteract(InteractDuration);
		}
	}
}
