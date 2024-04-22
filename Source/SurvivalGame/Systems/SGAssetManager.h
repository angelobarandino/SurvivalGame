// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static USGAssetManager& Get();
	
	virtual void StartInitialLoading() override;
};
