// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility.h"

#include "AbilitySystemComponent.h"

USGGameplayAbility::USGGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
