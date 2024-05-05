// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManagerComponent.h"
#include "CommonActivatableWidget.h"
#include "InteractionWidgetInterface.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionManagerComponent)


UInteractionManagerComponent::UInteractionManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInteractionManagerComponent::ShowInteraction(UInteractionDescriptor* InInteractionDescriptor)
{
	InteractionDescriptor = InInteractionDescriptor;
	
	if (OnShowInteraction.IsBound())
	{
		OnShowInteraction.Execute(InInteractionDescriptor);
	}
}

void UInteractionManagerComponent::HideInteraction()
{
	if (OnHideInteraction.IsBound())
	{
		OnHideInteraction.Execute(InteractionDescriptor.Get());	
	}

	InteractionDescriptor = nullptr;
	OnHoldProgress.Clear();
}

void UInteractionManagerComponent::PushContentToInteractionPrompt(const TSubclassOf<UCommonActivatableWidget> WidgetClass, const UObject* ContentData) const
{
	if (InteractionDescriptor)
	{
		const UUserWidget* InteractionWidget = InteractionDescriptor->InteractionWidget.Get();
		if (InteractionWidget && InteractionDescriptor->InteractionWidget->Implements<UInteractionWidgetInterface>())
		{
			if (IInteractionWidgetInterface* InteractionWidgetInterface = Cast<IInteractionWidgetInterface>(InteractionDescriptor->InteractionWidget))
			{
				InteractionWidgetInterface->PushContentToInteractionPrompt(WidgetClass, ContentData, InteractionDescriptor);
			}
		}
	}
}

void UInteractionManagerComponent::StartHoldInteract(const float HoldDuration)
{
	HoldTimerDuration = HoldDuration;
	GetWorldTimerManager().ClearTimer(HoldTimerHandle);
	GetWorldTimerManager().SetTimer(HoldTimerHandle, this, &UInteractionManagerComponent::HoldInteractProgress, HoldTimerRate, true);
}

void UInteractionManagerComponent::EndHoldInteract()
{
	OnHoldProgress.Broadcast(0.f);
	
	HoldTimerDuration = 0;
	HoldTimerProgress = 0;
	GetWorldTimerManager().ClearTimer(HoldTimerHandle);
}

void UInteractionManagerComponent::HoldInteractProgress()
{
	HoldTimerProgress += HoldTimerRate;

	const float Percentage = HoldTimerProgress / HoldTimerDuration;
	OnHoldProgress.Broadcast(FMath::Clamp(Percentage, 0.f, 1.f));
	
	if (FMath::IsNearlyEqual(Percentage, 1.f, 0.01f))
	{
		OnHoldCompleted.Broadcast();
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);
	}
}
