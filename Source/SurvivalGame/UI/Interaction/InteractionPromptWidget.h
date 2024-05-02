// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InteractionWidgetInterface.h"
#include "InteractionPromptWidget.generated.h"

class UCommonTextBlock;
class UCommonActivatableWidgetStack;
class UCommonActivatableWidget;

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInteractionPromptWidget : public UCommonUserWidget, public IInteractionWidgetInterface
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> InteractableName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> InteractionWidgetStack;

	virtual void PushContentToInteractionPrompt(TSubclassOf<UCommonActivatableWidget> WidgetClass, const UObject* ContentData) override;
};
