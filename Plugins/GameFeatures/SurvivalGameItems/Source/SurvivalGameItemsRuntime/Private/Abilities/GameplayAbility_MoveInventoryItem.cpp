// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_MoveInventoryItem.h"

#include "InventorySystem/InventoryManagerComponent.h"
#include "Pickups/IPickupable.h"
#include "Pickups/ItemPickupContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_MoveInventoryItem)

UGameplayAbility_MoveInventoryItem::UGameplayAbility_MoveInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameplayAbility_MoveInventoryItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthority(&CurrentActivationInfo))
	{
		MoveInventoryItem(TriggerEventData->Instigator, TriggerEventData->OptionalObject);
	}
}

void UGameplayAbility_MoveInventoryItem::MoveInventoryItem(const AActor* PlayerActor, const UObject* MoveItemObject)
{
	if (MoveItemObject == nullptr || PlayerActor == nullptr)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (const UMoveInventoryItemPayload* Data = Cast<UMoveInventoryItemPayload>(MoveItemObject))
	{
 		// If not Player, and Source and Target guids are valid, then were are moving items on a none player inventory
		if (!Data->bPlayerInventory && Data->SourceActorNetGUID.IsValid() && Data->TargetActorNetGUID.IsValid())
		{
			if (AActor* SourceActor = UPickupableStatics::FindActorByNetGUID<AItemPickupContainer>(GetWorld(), Data->SourceActorNetGUID))
			{
				const AActor* TargetActor = UPickupableStatics::FindActorByNetGUID<AItemPickupContainer>(GetWorld(), Data->TargetActorNetGUID);
				if (SourceActor == TargetActor)
				{
					if (UInventoryManagerComponent* SourceInventory = SourceActor->FindComponentByClass<UInventoryManagerComponent>())
					{
						SourceInventory->MoveInventoryItem(Data->SourceSlot, Data->TargetSlot);
						SourceActor->ForceNetUpdate();
					}
				}
			}
		}
		
		// If Player and Source guid is valid, then we're moving an item from external inventory to player's inventory
		else if (Data->bPlayerInventory && Data->SourceActorNetGUID.IsValid())
		{
			if (const AActor* SourceActor = UPickupableStatics::FindActorByNetGUID<AItemPickupContainer>(GetWorld(), Data->SourceActorNetGUID))
			{
				UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>();
				UInventoryManagerComponent* SourceInventory = SourceActor->FindComponentByClass<UInventoryManagerComponent>();
				check(PlayerInventory && SourceInventory);

				PlayerInventory->Server_AddInventoryItemFromOtherSource(Data->TargetSlot, Data->SourceSlot, SourceInventory);
			}
		}

		// if Player and Target guid is valid, then we're moving an item from player's inventory to external inventory
		else if (Data->bPlayerInventory && Data->TargetActorNetGUID.IsValid())
		{
			if (const AActor* TargetActor = UPickupableStatics::FindActorByNetGUID<AItemPickupContainer>(GetWorld(), Data->TargetActorNetGUID))
			{
				UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>();
				UInventoryManagerComponent* TargetInventory = TargetActor->FindComponentByClass<UInventoryManagerComponent>();
				check(PlayerInventory && TargetInventory);

				TargetInventory->Server_AddInventoryItemFromOtherSource(Data->TargetSlot, Data->SourceSlot, PlayerInventory);
			}
		}

		// if above condition fails, and it's Player's inventory
		else if (Data->bPlayerInventory)
		{
			if (UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>())
			{
				PlayerInventory->MoveInventoryItem(Data->SourceSlot, Data->TargetSlot);
			}
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}
