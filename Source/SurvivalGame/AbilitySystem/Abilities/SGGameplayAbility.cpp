// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility.h"

#include "AbilitySystemComponent.h"

USGGameplayAbility::USGGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

APlayerController* USGGameplayAbility::GetPlayerControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		return CurrentActorInfo->PlayerController.Get();
	}

	return nullptr;
}

APawn* USGGameplayAbility::GetPawnFromAvatarInfo() const
{
	return Cast<APawn>(GetAvatarActorFromActorInfo());
}

void USGGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateAbilityOnGranted(ActorInfo, Spec);
}

void USGGameplayAbility::TryActivateAbilityOnGranted(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting;
	
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && bActivateAbilityOnGranted)
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