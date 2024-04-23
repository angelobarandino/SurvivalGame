// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGameplayAbility_Interact.generated.h"

struct FInteractOption;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGGameplayAbility_Interact : public USGGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractionScanRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractionScanRate = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bShowDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DynamicInteractionGameplayEffectClass;
	
	UFUNCTION(BlueprintCallable)
	void ApplyInteractOptionsActivationRequirement(const TArray<FInteractOption>& InteractionOptions);

	UFUNCTION(BlueprintCallable)
	void ClearInteractOptionActivationRequirement();
	
private:
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
};
