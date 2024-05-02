// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGameplayAbility_InteractAction.generated.h"

class IInteractableTarget;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class SURVIVALGAME_API USGGameplayAbility_InteractAction : public USGGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility_InteractAction(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractDuration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta=(Categories="Ability.Interaction"))
	FGameplayTag InteractActiveGrantedTag;

protected:

	UFUNCTION(BlueprintCallable)
	AActor* GetActorFromInteractableTarget() const;
	
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IInteractableTarget> GetCurrentInteractableTarget() const;
};
