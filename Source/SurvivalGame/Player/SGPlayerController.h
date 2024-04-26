// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "SGPlayerController.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class SURVIVALGAME_API ASGPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	ASGPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};


