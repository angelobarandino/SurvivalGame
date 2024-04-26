// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionWidgetInterface.generated.h"

class UInteractionDescriptor;

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
};
