// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalGame/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGameplayAbility_Interact.generated.h"

class UInteractionDescriptor;
struct FInteractionDefinition;
class UInteractionManagerComponent;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DynamicInteractionGameplayEffectClass;
	
	UFUNCTION(BlueprintCallable)
	void ApplyInteractOptionActivationRequirement(const FInteractionDefinition& InteractionDefinition);

	UFUNCTION(BlueprintCallable)
	void ClearInteractOptionActivationRequirement();

	UFUNCTION(BlueprintCallable)
	void ShowInteractionPanel(const FInteractionDefinition& InteractionDefinition);
	
	UFUNCTION(BlueprintCallable)
	void HideInteractionPanel();

private:
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;

	UPROPERTY()
	TObjectPtr<UInteractionDescriptor> InteractionDescriptor;

	UFUNCTION()
	void GetInteractionGrantedTags(const FInteractionDefinition& InteractionDefinition, TArray<FGameplayTag>& OutGrantedTags) const;
};
