// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "SurvivalGame/Interactions/InteractionStatics.h"
#include "SurvivalGame/Interactions/InteractOption.h"

USGGameplayAbility_Interact::USGGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActivateAbilityOnGranted = true;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USGGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USGGameplayAbility_Interact::ApplyInteractOptionsActivationRequirement(const TArray<FInteractOption>& InteractionOptions)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC == nullptr)
	{
		return;
	}
	
	const AActor* AvatarActor = ASC->GetAvatarActor();
	if (AvatarActor == nullptr)
	{
		return;
	}
	
	for (const FInteractOption& InteractOption : InteractionOptions)
	{
		if (AActor* InteractableActor = UInteractionStatics::GetActorFromInteractableTarget(InteractOption.InteractableTarget))
		{
			if (const FGameplayAbilitySpec* AbilitySpecHandle = ASC->FindAbilitySpecFromClass(InteractOption.InteractionAbilityToGrant))
			{
				FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
				EffectContextHandle.SetAbility(AbilitySpecHandle->Ability);
				EffectContextHandle.AddSourceObject(AvatarActor);
				EffectContextHandle.AddActors({ InteractableActor });
				
				FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(
					DynamicInteractionGameplayEffectClass, 1, EffectContextHandle);

				FGameplayEffectSpec* Spec = EffectSpec.Data.Get();
				Spec->DynamicGrantedTags.AddTag(InteractOption.ActivationRequiredTag);
				FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec);
				ActiveGameplayEffectHandles.Add(EffectHandle);
			}
		}
	}
}

void USGGameplayAbility_Interact::ClearInteractOptionActivationRequirement()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC == nullptr)
	{
		return;
	}

	for (const FActiveGameplayEffectHandle& EffectHandle : ActiveGameplayEffectHandles)
	{
		ASC->RemoveActiveGameplayEffect(EffectHandle);
	}

	ActiveGameplayEffectHandles.Empty();
}
