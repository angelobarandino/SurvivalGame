// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility_InteractAction.h"

#include "SurvivalGame/Interactions/InteractionStatics.h"


USGGameplayAbility_InteractAction::USGGameplayAbility_InteractAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	InteractDuration = 0.f;
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
