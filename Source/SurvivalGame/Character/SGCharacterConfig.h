// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGCharacterConfig.generated.h"

class USGInputConfig;
class ASGCharacter;
/**
 * 
 */
UCLASS(BlueprintType, Const, meta = (DisplayName = "Character Config"))
class SURVIVALGAME_API USGCharacterConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USGCharacterConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SurvivalGame|Character")
	TSubclassOf<ASGCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<const USGInputConfig> InputConfig;
};
