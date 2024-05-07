// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/Interactions/Abilities/SGGameplayAbility_InteractAction.h"
#include "GameplayAbility_TransferInventoryItem.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SURVIVALGAMEITEMSRUNTIME_API UGameplayAbility_TransferInventoryItem : public USGGameplayAbility_InteractAction
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnReleaseInput(float TimeHeld);

};
