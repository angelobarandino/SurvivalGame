// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGameplayAbility_Interact.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAMEINTERACTIONRUNTIME_API USGGameplayAbility_Interact : public USGGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};
