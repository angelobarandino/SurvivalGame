// Fill out your copyright notice in the Description page of Project Settings.


#include "IPickupable.h"
#include "InventorySystem/InventoryManagerComponent.h"

FInventoryPickup IPickupable::GetPickupInventory() const
{
	return FInventoryPickup();
}

TScriptInterface<IPickupable> UPickupableStatics::GetPickupableFromActor(AActor* Actor)
{
	const TScriptInterface<IPickupable> PickupableActor(Actor);
	
	if (PickupableActor)
	{
		return PickupableActor;
	}
	
	return TScriptInterface<IPickupable>();
}

void UPickupableStatics::AddPickupToInventory(APlayerController* PlayerController, const TScriptInterface<IPickupable> Pickup)
{
	if (PlayerController == nullptr)
	{
		return;
	}
	
	if (UInventoryManagerComponent* InventoryManager = PlayerController->FindComponentByClass<UInventoryManagerComponent>())
	{
		const FInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for (const FPickupInstance& Instance : PickupInventory.Instances)
		{
			Pickup->OnPickupAddedToInventory(Instance, InventoryManager->AddInventorItem(Instance.ItemDef, Instance.ItemStack));
		}
	}
}
