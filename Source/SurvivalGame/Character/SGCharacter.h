// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "SGCharacter.generated.h"

class USGPawnManagerComponent;
class UAttributeSet;
class USGAttributeSet;
class USGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASGCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USGPawnManagerComponent> PawnManagerComponent;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;

};
