// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "SGPlayerController.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class USGInputConfig;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ASGPlayerController(const FObjectInitializer& ObjectInitializer);
	
};


