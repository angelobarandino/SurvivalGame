﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SurvivalGame/Interactions/InteractableTarget.h"
#include "SurvivalGame/Interactions/InteractionStatics.h"
#include "UObject/Object.h"
#include "InteractionDescriptor.generated.h"


class UInteractionDescriptor;

UCLASS(BlueprintType)
class SURVIVALGAME_API UInteractionDescriptor : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Interaction Descriptor")
	FName GetInteractableName() const { return InteractableName; }
	void SetInteractableObjectName(const FName& Name)
	{
		InteractableName = Name;
	}
	
	TSoftClassPtr<UUserWidget> GetInteractionWidgetClass() const { return InteractionWidgetClass;}
	void SetInteractionWidgetClass(const TSoftClassPtr<UUserWidget>& InInteractionWidgetClass)
	{
		InteractionWidgetClass = InInteractionWidgetClass;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Interaction Descriptor")
	AActor* GetInteractableActor() const
	{
		return UInteractionStatics::GetActorFromInteractableTarget(InteractableTarget);
	}

	UFUNCTION(BlueprintCallable, Category = "Interaction Descriptor")
	TScriptInterface<IInteractableTarget> GetInteractableTarget() const { return InteractableTarget; }
	void SetInteractableTarget(const TScriptInterface<IInteractableTarget>& InInteractableTarget)
	{
		InteractableTarget = InInteractableTarget;
	}
	
	FVector GetWorldLocation() const { return WorldLocation; }
	void SetWorldLocation(const FVector& InWorldLocation)
	{
		WorldLocation = InWorldLocation;
	}

	FVector2D GetScreenSpaceOffset() const { return ScreenSpaceOffset; }

	UFUNCTION(BlueprintCallable, Category = "Interaction Descriptor")
	void SetScreenSpaceOffset(const FVector2D& Offset)
	{
		ScreenSpaceOffset = Offset;
	}

	UFUNCTION(BlueprintCallable, Category = "Interaction Descriptor")
	TArray<FInteractOption> GetInteractionOption() const { return InteractOptions; }
	void SetInteractionOption(const TArray<FInteractOption>& InInteractOptions)
	{
		InteractOptions = InInteractOptions;
	}

	UPROPERTY()
	TWeakObjectPtr<UUserWidget> InteractionWidget;
	
private:
	UPROPERTY()
	FName InteractableName;
	
	UPROPERTY()
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;
	
	UPROPERTY()
	TScriptInterface<IInteractableTarget> InteractableTarget;

	UPROPERTY()
	TArray<FInteractOption> InteractOptions;

	UPROPERTY()
	FVector WorldLocation;

	UPROPERTY()
	FVector2D ScreenSpaceOffset;
};
