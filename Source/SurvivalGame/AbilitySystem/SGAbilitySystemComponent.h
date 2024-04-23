// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAvatarActorSetDelegate);

UCLASS()
class SURVIVALGAME_API USGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	USGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	FOnAvatarActorSetDelegate OnAvatarActorSet;
	void CallOrAddOnAvatarActorSet(FOnAvatarActorSetDelegate::FDelegate&& Delegate);

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};

