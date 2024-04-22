// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "SGWorldSettings.generated.h"

class USGExperienceDefinition;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	ASGWorldSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FPrimaryAssetId GetDefaultGameplayExperience() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSoftClassPtr<USGExperienceDefinition> DefaultGameplayExperience;
};
