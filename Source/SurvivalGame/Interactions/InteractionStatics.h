// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractOption.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionStatics.generated.h"

class UCommonActivatableWidget;
class IInteractableTarget;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SetActorHighlightEnable(const AActor* Actor, bool bEnable);

	static bool FindNearestIntractableTargetFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TScriptInterface<IInteractableTarget>& OutInteractableTarget);

	static FVector GetInteractableActorWorldLocation(const TScriptInterface<IInteractableTarget>& InteractableTarget);

	static void SetInteractOptionData(FInteractOption& InteractOption);

	UFUNCTION(BlueprintCallable, Category = "Interaction Statics")
	static TScriptInterface<IInteractableTarget> GetActiveInteractableTarget(const APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Interaction Statics")
	static AActor* GetActorFromInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableTarget);

	UFUNCTION(BlueprintCallable, Category = "Interaction Statics")
	static void CallInteractInputActionActivate(APlayerController* PlayerController, const FGameplayTag InputTag, const FGameplayTag InteractOptionTag);

	UFUNCTION(BlueprintCallable, Category = "Interaction Statics")
	static void PushContentToInteractionPrompt(APlayerController* PlayerController, TSubclassOf<UCommonActivatableWidget> WidgetClass);
};
