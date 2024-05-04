// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "SGPlayerController.generated.h"

struct FGameplayEventData;
struct FGameplayTag;
/**
 * 
 */
UCLASS(Config = Game)
class SURVIVALGAME_API ASGPlayerController : public ACommonPlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASGPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};


