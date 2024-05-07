// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/InventoryManagerComponent.h"
#include "Pickups/IPickupable.h"
#include "Pickups/ItemPickup.h"
#include "Pickups/PickupItemCollection.h"


TScriptInterface<IPickupable> UPickupableStatics::GetPickupableFromActor(AActor* Actor)
{
	const TScriptInterface<IPickupable> PickupableActor(Actor);
	
	if (PickupableActor)
	{
		return PickupableActor;
	}
	
	return TScriptInterface<IPickupable>();
}

void UPickupableStatics::SpawnItemPickupFromItemInstance(const TSubclassOf<AItemPickup> ItemPickupClass, APawn* OwnerPawn, const UInventoryItemInstance* ItemInstance, const FTransform& Transform)
{
	check(ItemPickupClass);
	check(OwnerPawn);
	check(ItemInstance);

	UWorld* World = OwnerPawn->GetWorld();
	if (AItemPickup* ItemPickup = World->SpawnActorDeferred<AItemPickup>(ItemPickupClass, Transform, nullptr, OwnerPawn))
	{
		ItemPickup->SetNetDormancy(DORM_DormantAll);
		ItemPickup->SetPickupItem(ItemInstance->GetItemDef(), ItemInstance->GetItemCount());
		ItemPickup->FinishSpawning(Transform);
	}
}

bool UPickupableStatics::AddAllItemPickupToInventory(APlayerController* PlayerController, const TScriptInterface<IPickupable> Pickup)
{
	if (PlayerController == nullptr || Pickup.GetObject() == nullptr)
	{
		return false;
	}
	
	if (UInventoryManagerComponent* InventoryManager = PlayerController->FindComponentByClass<UInventoryManagerComponent>())
	{
		const TArray<FPickupItemEntry> PickupInstances = Pickup->GetPickupItems();

		FPickupItemHandle PickupItemHandle;
		PickupItemHandle.AddItemResults.Reserve(PickupInstances.Num());
		
		for (const FPickupItemEntry& Entry : PickupInstances)
		{
			PickupItemHandle.AddResult(Entry.InstanceId, InventoryManager->AddInventorItem(Entry.ItemDef, Entry.ItemStack));
		}
		
		return Pickup->OnPickupAddedToInventory(PickupItemHandle, PlayerController);
	}

	return false;
}

void UPickupableStatics::TakeItem(APlayerController* PlayerController, TScriptInterface<IPickupable> Pickup, const int32 SlotIndex)
{
	if (PlayerController == nullptr || Pickup.GetObject() == nullptr || SlotIndex < 0)
	{
		return;
	}

	if (UInventoryManagerComponent* InventoryManager = PlayerController->FindComponentByClass<UInventoryManagerComponent>())
	{
		const TArray<FPickupItemEntry> PickupItems = Pickup->GetPickupItems();

		FPickupItemHandle PickupItemHandle;
		for (const FPickupItemEntry& PickupItem : PickupItems)
		{
			if (PickupItem.ItemSlot == SlotIndex)
			{
				PickupItemHandle.AddResult(PickupItem.InstanceId, InventoryManager->AddInventorItem(PickupItem.ItemDef, PickupItem.ItemStack));
				break;
			}
		}

		Pickup->OnPickupAddedToInventory(PickupItemHandle, PlayerController);
	}
}

