#include "AbilityTasks/AbilityTask_WaitForInteractableTarget.h"

#include "InteractableTarget.h"
#include "InteractionStatics.h"
#include "Physics/SGCollisionChannels.h"

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

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

void UAbilityTask_WaitForInteractableTarget::OnDestroy(bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitForInteractableTarget::PerformTrace()
{
	const UWorld* World = GetWorld();
	
	AActor* AvatarActor = GetAvatarActor();
	if (AvatarActor == nullptr)
	{
		return;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_WaitForInteractableTarget), false);
	Params.AddIgnoredActors(ActorsToIgnore);
	
	const FVector Position = (AvatarActor->GetActorLocation() + FVector::DownVector * 50.f) + (AvatarActor->GetActorRotation().Vector() * 50.f);

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

void UAbilityTask_WaitForInteractableTarget::FoundInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableObject)
{
	if (IsValid(ActiveInteractableTarget.GetObject()))
	{
		if (ActiveInteractableTarget == InteractableObject)
		{
			return;
		}

		LostInteractable.Broadcast();
		ActiveInteractableTarget->UnHighlightActor();
	}

	ActiveInteractableTarget = InteractableObject;
	ActiveInteractableTarget->HighlightActor();
	FoundInteractable.Broadcast();
}

void UAbilityTask_WaitForInteractableTarget::LostInteractableTarget()
{
	if (ActiveInteractableTarget == nullptr)
	{
		return;
	}
	
	if (IsValid(ActiveInteractableTarget.GetObject()))
	{
		ActiveInteractableTarget->UnHighlightActor();
	}
	
	LostInteractable.Broadcast();
	ActiveInteractableTarget = nullptr;
}
