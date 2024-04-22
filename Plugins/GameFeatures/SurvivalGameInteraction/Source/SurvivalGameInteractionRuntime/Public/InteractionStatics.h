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
class SURVIVALGAMEINTERACTIONRUNTIME_API UInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool FindNearestIntractableTargetFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TScriptInterface<IInteractableTarget>& OutInteractableTarget);
};
