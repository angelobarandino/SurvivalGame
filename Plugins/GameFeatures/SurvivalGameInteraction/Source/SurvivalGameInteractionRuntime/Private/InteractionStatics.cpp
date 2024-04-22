// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionStatics.h"

#include "InteractableTarget.h"


bool UInteractionStatics::FindNearestIntractableTargetFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TScriptInterface<IInteractableTarget>& OutInteractableTarget)
{
	if (OverlapResults.Num() == 0)
	{
		return false;
	}
	
	//@TODO: Implement finding the nearest interactable target
	for (FOverlapResult OverlapResult : OverlapResults)
	{
		AActor* OverlapActor = OverlapResult.GetActor();
		
		TScriptInterface<IInteractableTarget> InteractableActor(OverlapActor);
		if (InteractableActor)
		{
			OutInteractableTarget = InteractableActor;
			return true;
		}
		
		TScriptInterface<IInteractableTarget> InteractableComponent(OverlapActor->FindComponentByInterface<UInteractableTarget>());
		if (InteractableComponent)
		{
			OutInteractableTarget = InteractableComponent;
			return true;
		}
	}

	return false;
}
