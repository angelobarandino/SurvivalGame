// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility.h"

#include "AbilitySystemComponent.h"

USGGameplayAbility::USGGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USGGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (bActivateAbilityOnGranted && ActorInfo && !Spec.IsActive())
	{
		if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
		{
			ASC->TryActivateAbility(Spec.Handle);
		}
	}
}

void USGGameplayAbility::OnAvatarActorSet()
{
	
}

void USGGameplayAbility::ExternalEndAbility()
{
	check(CurrentActorInfo);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}