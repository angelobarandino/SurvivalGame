// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAbilitySystemComponent.h"

#include "Abilities/SGGameplayAbility.h"

USGAbilitySystemComponent::USGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USGAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (USGGameplayAbility* Ability = Cast<USGGameplayAbility>(AbilitySpec.Ability))
		{
			Ability->TryActivateAbilityOnGranted(AbilityActorInfo.Get(), AbilitySpec);
			
			if (Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				TArray<UGameplayAbility*> AbilitySpecsInstances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilitySpecsInstance : AbilitySpecsInstances)
				{
					if (USGGameplayAbility* AbilityInstance = Cast<USGGameplayAbility>(AbilitySpecsInstance))
					{
						AbilityInstance->OnAvatarActorSet();
					}
				}
			}
			else
			{
				Ability->OnAvatarActorSet();
			}
		}
	}

	OnAvatarActorSet.Broadcast();
}

void USGAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void USGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void USGAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USGAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void USGAbilitySystemComponent::CallOrAddOnAvatarActorSet(FOnAvatarActorSetDelegate::FDelegate&& Delegate)
{
	if (Cast<APawn>(GetAvatarActor()))
	{
		Delegate.Execute();
	}
	else
	{
		OnAvatarActorSet.Add(MoveTemp(Delegate));
	}
}

void USGAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if (const USGGameplayAbility* GameplayAbility = Cast<USGGameplayAbility>(AbilitySpec.Ability))
	{
		AbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->InputTag);	
	}

	Super::OnGiveAbility(AbilitySpec);
}
