// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGExperienceDefinition.generated.h"

class USGCharacterConfig;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class USGExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~End of UObject interface

	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FString> GameFeaturesToEnable;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TObjectPtr<const USGCharacterConfig> CharacterConfig;
	
};
