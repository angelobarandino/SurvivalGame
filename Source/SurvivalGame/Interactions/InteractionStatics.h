// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionStatics.generated.h"

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
	static AActor* GetActorFromInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableTarget);
};
