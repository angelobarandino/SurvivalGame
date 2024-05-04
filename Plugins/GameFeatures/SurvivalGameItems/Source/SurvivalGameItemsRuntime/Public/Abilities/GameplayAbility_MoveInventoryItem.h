// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_MoveInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UGameplayAbility_MoveInventoryItem : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_MoveInventoryItem(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void MoveInventoryItem(const AActor* PlayerActor, const UObject* MoveItemObject);
};
