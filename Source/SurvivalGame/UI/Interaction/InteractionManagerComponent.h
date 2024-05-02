// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDescriptor.h"
#include "Components/ControllerComponent.h"
#include "InteractionManagerComponent.generated.h"

class UCommonActivatableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoldToInteractProgress, float, Progress);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALGAME_API UInteractionManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UInteractionManagerComponent(const FObjectInitializer& ObjectInitializer);

	void CallInteractInputActionActivate(const FGameplayTag InputTag, const FGameplayTag InteractOptionTag) const;
	void ShowInteraction(UInteractionDescriptor* InInteractionDescriptor);
	void HideInteraction();
	void PushContentToInteractionPrompt(const TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	DECLARE_DELEGATE_OneParam(FInteractionEvent, UInteractionDescriptor*);
	FInteractionEvent OnShowInteraction;
	FInteractionEvent OnHideInteraction;

	UFUNCTION(BlueprintCallable)
	void StartHoldInteract(const float HoldDuration);

	UFUNCTION(BlueprintCallable)
	void EndHoldInteract();

	UPROPERTY(BlueprintAssignable)	
	FHoldToInteractProgress OnHoldProgress;
	
private:
	UPROPERTY()
	TObjectPtr<UInteractionDescriptor> InteractionDescriptor;

	FTimerHandle HoldTimerHandle;
	float HoldTimerDuration = 0.f;
	float HoldTimerProgress = 0.f;
	float HoldTimerRate = 0.01f;

	void HoldInteractProgress();
};

