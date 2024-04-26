// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionLibrary.generated.h"

class UInteractionManagerComponent;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	static UInteractionManagerComponent* GetInteractionManager(AController* Controller);

};
