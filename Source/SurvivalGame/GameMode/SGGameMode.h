// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "SGExperienceDefinition.h"
#include "SGGameMode.generated.h"

class USGCharacterConfig;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	ASGGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void InitGameState() override;

protected:
	const USGCharacterConfig* GetCharacterConfigForController(const AController* InController) const;
	bool IsExperienceLoaded() const;
	void LoadGameExperience() const;
	void OnGameplayExperienceLoaded(const USGExperienceDefinition* ExperienceDefinition);
};
