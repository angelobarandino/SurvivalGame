// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/InventoryManagerComponent.h"
#include "InventorySystem/InventoryTypes.h"
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
		ItemPickup->SetPickupItems({
			FPickupItemEntry(ItemInstance->GetItemDef(), ItemInstance->GetItemCount())
		});
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

		TMap<FGuid, FAddInventoryItemResult> ResultMap;
		ResultMap.Reserve(PickupInstances.Num());
		
		for (const FPickupItemEntry& Entry : PickupInstances)
		{
			ResultMap.Add(Entry.InstanceId, InventoryManager->AddInventorItem(Entry.ItemDef, Entry.ItemStack));
		}
		
		return Pickup->OnPickupAddedToInventory(ResultMap, PlayerController);
	}

	return false;
}
