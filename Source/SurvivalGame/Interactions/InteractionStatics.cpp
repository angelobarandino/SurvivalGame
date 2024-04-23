// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionStatics.h"

#include "InteractableTarget.h"

void UInteractionStatics::SetActorHighlightEnable(const AActor* Actor, const bool bEnable)
{
	if (Actor == nullptr)
	{
		return;
	}

	TArray<UStaticMeshComponent*> StaticMeshComponents;
	Actor->GetComponents(UStaticMeshComponent::StaticClass(), StaticMeshComponents);
	for (UStaticMeshComponent* StaticMeshComp : StaticMeshComponents)
	{
		StaticMeshComp->SetRenderCustomDepth(bEnable);
	}
}

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

AActor* UInteractionStatics::GetActorFromInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableTarget)
{
	if (UObject* InteractableObjectRef = InteractableTarget.GetObject())
	{
		if (AActor* Actor = Cast<AActor>(InteractableObjectRef))
		{
			return Actor;
		}

		if (const UActorComponent* ActorComponent = Cast<UActorComponent>(InteractableObjectRef))
		{
			return ActorComponent->GetOwner();
		}
	}

	return nullptr;
}
