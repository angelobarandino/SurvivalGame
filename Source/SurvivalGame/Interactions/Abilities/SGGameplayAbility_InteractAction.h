// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGameplayAbility_InteractAction.generated.h"

class UAbilityTask_WaitDelay;
class UAbilityTask_WaitInputRelease;
class IInteractableTarget;

/**
 * ///////////////////////////////////////////////////////////////////////////////////////////////////////
 * USGGameplayAbility_InteractAction
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class SURVIVALGAME_API USGGameplayAbility_InteractAction : public USGGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility_InteractAction(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta=(Categories="Ability.Interaction"))
	FGameplayTag InteractActiveGrantedTag;

protected:

	UFUNCTION(BlueprintCallable)
	AActor* GetActorFromInteractableTarget() const;
	
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IInteractableTarget> GetCurrentInteractableTarget() const;
};


/**
 * ///////////////////////////////////////////////////////////////////////////////////////////////////////
 * USGGameplayAbility_InteractDuration
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class SURVIVALGAME_API USGGameplayAbility_InteractDuration : public USGGameplayAbility_InteractAction
{
	GENERATED_BODY()
	
public:
	USGGameplayAbility_InteractDuration(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float WaitHoldThreshold;
	
protected:
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHoldToInteractCancelled();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHoldToInteractProgress(float Progress);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHoldToInteractCompleted();
	
private:

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> WaitDelayTask;

	UFUNCTION()
	void EndHoldInteract() const;

	UFUNCTION()
	void BeginHoldToInteract();

};