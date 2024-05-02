// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionStatics.h"

#include "InteractableTarget.h"
#include "Components/InteractableScannerComponent.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SurvivalGame/UI/Interaction/InteractionManagerComponent.h"

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

FVector UInteractionStatics::GetInteractableActorWorldLocation(const TScriptInterface<IInteractableTarget>& InteractableTarget)
{
	if (UObject* InteractableObjectRef = InteractableTarget.GetObject())
	{
		if (const AActor* Actor = Cast<AActor>(InteractableObjectRef))
		{
			return Actor->GetActorLocation();
		}

		if (const UActorComponent* ActorComponent = Cast<UActorComponent>(InteractableObjectRef))
		{
			if (const AActor* Actor = ActorComponent->GetOwner())
			{
				return Actor->GetActorLocation();
			}
		}
	}

	return FVector::Zero();
}

void UInteractionStatics::SetInteractOptionData(FInteractOption& InteractOption)
{
	if (InteractOption.InteractionAbilityToGrant)
	{
		if (const USGGameplayAbility_InteractAction* Ability = GetDefault<USGGameplayAbility_InteractAction>(InteractOption.InteractionAbilityToGrant))
		{
			InteractOption.InteractInputTag = Ability->InputTag;
			InteractOption.InteractDuration = Ability->InteractDuration;
		}
	}
}

TScriptInterface<IInteractableTarget> UInteractionStatics::GetActiveInteractableTarget(const APawn* Pawn)
{
	if (Pawn)
	{
		if (const UInteractableScannerComponent* InteractableScanner = Pawn->FindComponentByClass<UInteractableScannerComponent>())
		{
			return InteractableScanner->GetActiveInteractableTarget();
		}
	}

	return TScriptInterface<IInteractableTarget>();
}

void UInteractionStatics::PushContentToInteractionPrompt(APlayerController* PlayerController, const TSubclassOf<UCommonActivatableWidget> WidgetClass, const UObject* ContentData)
{
	if (PlayerController)
	{
		if (const UInteractionManagerComponent* InteractionManager = PlayerController->FindComponentByClass<UInteractionManagerComponent>())
		{
			InteractionManager->PushContentToInteractionPrompt(WidgetClass, ContentData);
		}
	}
}
