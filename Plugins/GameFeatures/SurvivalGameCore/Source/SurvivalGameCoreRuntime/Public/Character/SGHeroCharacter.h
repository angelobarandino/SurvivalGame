// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/Character/SGCharacter.h"
#include "SGHeroCharacter.generated.h"

class USGHeroComponent;
/**
 * 
 */
UCLASS()
class SURVIVALGAMECORERUNTIME_API ASGHeroCharacter : public ASGCharacter
{
	GENERATED_BODY()

public:
	ASGHeroCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Components")
	TObjectPtr<USGHeroComponent> HeroComponent;
	
};
