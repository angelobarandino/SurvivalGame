// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "InteractionWidgetInterface.generated.h"

class UInteractionDescriptor;
class UCommonActivatableWidget;

UINTERFACE(BlueprintType)
class UInteractionWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class SURVIVALGAME_API IInteractionWidgetInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void BindInteraction(UInteractionDescriptor* Interaction);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void UnbindInteraction(UInteractionDescriptor* Interaction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteractInputActionActivate(const FGameplayTag InteractInputTag, const FGameplayTag InteractOptionTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void PushContentToInteractionPrompt(TSubclassOf<UCommonActivatableWidget> WidgetClass);
};
