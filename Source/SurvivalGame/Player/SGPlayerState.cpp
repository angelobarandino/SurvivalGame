// Fill out your copyright notice in the Description page of Project Settings.


#include "SGPlayerState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/GameFrameworkComponentManager.h"
#include "SurvivalGame/AbilitySystem/SGAbilitySystemComponent.h"
#include "SurvivalGame/AbilitySystem/Attributes/SGAttributeSet.h"

const FName ASGPlayerState::NAME_AbilitiesReady("AbilitiesReady");

ASGPlayerState::ASGPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<USGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);

	Attributes = CreateDefaultSubobject<USGAttributeSet>("AttributeSet");
}

void ASGPlayerState::InitAbilityActorInfo(APawn* AvatarActor)
{
	AbilitySystemComponent->InitAbilityActorInfo(this, AvatarActor);

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_AbilitiesReady);
}

UAbilitySystemComponent* ASGPlayerState::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

UAttributeSet* ASGPlayerState::GetAttributeSet() const
{
	return Cast<UAttributeSet>(Attributes);
}

void ASGPlayerState::Server_ActivateActorAbilityByEvent_Implementation(const FGameplayTag& EventTag, const FGameplayEventData& Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, Payload);
}
