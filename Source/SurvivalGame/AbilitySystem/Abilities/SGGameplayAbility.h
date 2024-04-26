// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	bool bActivateAbilityOnGranted = false;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerController* GetPlayerControllerFromActorInfo() const;
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void OnAvatarActorSet();
	
	void ExternalEndAbility();
};
