// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class SURVIVALGAME_API USGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation", meta=(Categories="InputTag.Ability"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	bool bActivateAbilityOnGranted = false;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerController* GetPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APawn* GetPawnFromAvatarInfo() const;
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void TryActivateAbilityOnGranted(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	virtual void OnAvatarActorSet();
	
	void ExternalEndAbility();

};
