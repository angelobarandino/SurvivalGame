// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "SGPlayerState.generated.h"

struct FGameplayEventData;
struct FGameplayTag;
class UAttributeSet;
class USGAttributeSet;
class USGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASGPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_AbilitiesReady;
	
	void InitAbilityActorInfo(APawn* AvatarActor);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const;

	
	UFUNCTION(NetMulticast, Reliable)
	void Server_ActivateActorAbilityByEvent(const FGameplayTag& EventTag, const FGameplayEventData& Payload);
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USGAttributeSet> Attributes;
};
