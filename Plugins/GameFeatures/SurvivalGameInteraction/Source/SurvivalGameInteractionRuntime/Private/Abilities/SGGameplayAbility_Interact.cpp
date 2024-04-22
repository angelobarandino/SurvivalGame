// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SGGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"

USGGameplayAbility_Interact::USGGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActivateAbilityOnGranted = true;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USGGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
     	if (ASC && ASC->GetOwnerRole() == ROLE_Authority)
     	{
     		
     	}
}

