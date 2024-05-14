// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableScannerComponent.h"

#include "SurvivalGame/Interactions/InteractableTarget.h"
#include "SurvivalGame/Interactions/InteractionStatics.h"
#include "SurvivalGame/Physics/SGCollisionChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractableScannerComponent)

UInteractableScannerComponent::UInteractableScannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// SetIsReplicatedByDefault(false);
}

TScriptInterface<IInteractableTarget> UInteractableScannerComponent::GetActiveInteractableTarget() const
{
	return ActiveInteractableTarget;
}

void UInteractableScannerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

void UInteractableScannerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	Super::EndPlay(EndPlayReason);
}

void UInteractableScannerComponent::PerformTrace()
{
	const UWorld* World = GetWorld();
	
	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UInteractableScannerComponent), false);
	Params.AddIgnoredActors(ActorsToIgnore);
	
	const FVector Position = (Owner->GetActorLocation() + FVector::DownVector * 50.f) + (Owner->GetActorRotation().Vector() * 50.f);

	TArray<FOverlapResult> OverlapResults;
	World->OverlapMultiByChannel(OverlapResults, Position, FQuat::Identity, ECC_Interaction, FCollisionShape::MakeSphere(InteractionScanRadius));

	TScriptInterface<IInteractableTarget> FoundInteractableObject;
	if (UInteractionStatics::FindNearestIntractableTargetFromOverlapResults(OverlapResults, FoundInteractableObject))
	{
		FoundInteractableTarget(FoundInteractableObject);
	}
	else
	{
		LostInteractableTarget();
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), Position, InteractionScanRadius, 12, FColor::Yellow, false, InteractionScanRate, 0, 1);
	}
#endif
}

void UInteractableScannerComponent::ToggleInteractableHighlight(const TScriptInterface<IInteractableTarget>& InteractableObject, const bool bIsHighligh) const
{
	if (const APawn* Owner = Cast<APawn>(GetOwner()))
	{
		if (Owner->IsLocallyControlled())
		{
			if (const AActor* InteractableActor = UInteractionStatics::GetActorFromInteractableTarget(InteractableObject))
			{
				UInteractionStatics::SetActorHighlightEnable(InteractableActor, bIsHighligh);
			}
		}
	}
}

void UInteractableScannerComponent::FoundInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableObject)
{
	if (IsValid(ActiveInteractableTarget.GetObject()))
	{
		if (ActiveInteractableTarget == InteractableObject)
		{
			return;
		}

		LostInteractable.Broadcast(TScriptInterface<IInteractableTarget>());
		ToggleInteractableHighlight(ActiveInteractableTarget, false);
	}

	ActiveInteractableTarget = InteractableObject;

	ToggleInteractableHighlight(InteractableObject, true);
	
	FoundInteractable.Broadcast(ActiveInteractableTarget);
}

void UInteractableScannerComponent::LostInteractableTarget()
{
	if (ActiveInteractableTarget == nullptr)
	{
		return;
	}
	
	if (IsValid(ActiveInteractableTarget.GetObject()))
	{
		ToggleInteractableHighlight(ActiveInteractableTarget, false);
	}
	
	LostInteractable.Broadcast(TScriptInterface<IInteractableTarget>());
	ActiveInteractableTarget = nullptr;
}
