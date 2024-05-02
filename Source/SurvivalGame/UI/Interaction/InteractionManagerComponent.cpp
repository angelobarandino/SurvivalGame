// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManagerComponent.h"
#include "CommonActivatableWidget.h"
#include "InteractionWidgetInterface.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionManagerComponent)


UInteractionManagerComponent::UInteractionManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInteractionManagerComponent::CallInteractInputActionActivate(const FGameplayTag InputTag, const FGameplayTag InteractOptionTag) const
{
	if (InteractionDescriptor)
	{
		UUserWidget* InteractionWidget = InteractionDescriptor->InteractionWidget.Get();
		if (InteractionWidget && InteractionDescriptor->InteractionWidget->Implements<UInteractionWidgetInterface>())
		{
			IInteractionWidgetInterface::Execute_OnInteractInputActionActivate(InteractionWidget, InputTag, InteractOptionTag);
		}
	}
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

void UInteractionManagerComponent::PushContentToInteractionPrompt(const TSubclassOf<UCommonActivatableWidget> WidgetClass) const
{
	if (InteractionDescriptor)
	{
		UUserWidget* InteractionWidget = InteractionDescriptor->InteractionWidget.Get();
		if (InteractionWidget && InteractionDescriptor->InteractionWidget->Implements<UInteractionWidgetInterface>())
		{
			IInteractionWidgetInterface::Execute_PushContentToInteractionPrompt(InteractionWidget, WidgetClass);
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

	OnHoldProgress.Broadcast(FMath::Clamp(HoldTimerProgress, 0.f, 1.f));
	
	if (HoldTimerProgress >= HoldTimerDuration)
	{
		EndHoldInteract();
	}
}
