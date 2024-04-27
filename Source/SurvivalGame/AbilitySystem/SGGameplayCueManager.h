// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "SGGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	USGGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static USGGameplayCueManager* Get();
	
	void RefreshGameplayCuePrimaryAsset();
};
