// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_TransferInventoryItem.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Components/PawnItemManagerComponent.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "Pickups/IPickupable.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_TransferInventoryItem)


void UGameplayAbility_TransferInventoryItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	Task->OnRelease.AddDynamic(this, &ThisClass::OnReleaseInput);
	Task->ReadyForActivation();
}

void UGameplayAbility_TransferInventoryItem::OnReleaseInput(float TimeHeld)
{
	if (TimeHeld < 0.2f)
	{
		if (HasAuthority(&CurrentActivationInfo))
		{
			const FInventoryFocusedItem& FocusedItem = UPawnItemManagerStatics::GetFocusedInventoryItem(GetPawnFromAvatarInfo());
			if (FocusedItem.HasValidFocused())
			{
				AActor* PlayerActor = GetPlayerControllerFromActorInfo();
				AActor* TargetActor = GetActorFromInteractableTarget();
				
				if (PlayerActor && TargetActor)
				{
					UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>();
					UInventoryManagerComponent* TargetInventory =TargetActor->FindComponentByClass<UInventoryManagerComponent>();
					check(PlayerInventory && TargetInventory);

					// if we are hovering on an item in players inventory, then we add to target pickupable actor
					if (FocusedItem.bIsPlayer)
					{
						TargetInventory->AddInventoryItemFromOtherSource(FocusedItem.ItemSlot, PlayerInventory);
					}
					
					// if we are hovering on an item in a pickupable actor, then we add it to players inventory
					else
					{
						PlayerInventory->AddInventoryItemFromOtherSource(FocusedItem.ItemSlot, TargetInventory);
						
						if (IPickupable* PickupableTargetActor = Cast<IPickupable>(TargetActor))
						{
							PickupableTargetActor->TryDestroyPickupable();
						}
					}
				}
			}
		}
	}

	ExternalEndAbility();
}
