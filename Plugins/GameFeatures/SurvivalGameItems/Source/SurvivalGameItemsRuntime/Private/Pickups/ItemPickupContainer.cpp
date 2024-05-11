// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ItemPickupContainer.h"

#include "InventorySystem/InventoryManagerComponent.h"
#include "InventorySystem/InventoryTypes.h"


AItemPickupContainer::AItemPickupContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	NetUpdateFrequency = 15.f;
	MinNetUpdateFrequency = 5.f;
	NetCullDistanceSquared = 1000000;
	NetDormancy = DORM_Initial;

	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManagerComponent");
}

void AItemPickupContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AItemPickupContainer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		InventoryManager->MaxInventorySize = MaxInventorySize;

		for (const FPickupItemEntry& PickupItem : Pickups.Items)
		{
			InventoryManager->AddInitialInventoryItem(PickupItem.ItemDef, PickupItem.ItemStack);
		}
	}
}

void AItemPickupContainer::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FPickupItemEntry> AItemPickupContainer::GetPickupItems()
{
	TArray<FPickupItemEntry> PickupItems;
	TArray<UInventoryItemInstance*> Items = InventoryManager->GetItems(true);
	for (const UInventoryItemInstance* ItemInstance : Items)
	{
		FPickupItemEntry& Entry = PickupItems.AddDefaulted_GetRef();
		Entry.ItemDef = ItemInstance->GetItemDef();
		Entry.ItemStack = ItemInstance->GetItemCount();
		Entry.ItemSlot = ItemInstance->GetItemSlot();
		Entry.ItemInstanceRef = ItemInstance;
	}

	Pickups.Items = PickupItems;
	return PickupItems;
}

bool AItemPickupContainer::TryDestroyPickupable()
{
	if (InventoryManager->GetItems().Num() == 0 && bDestroyOnPickup)
	{
		Destroy(true, true);
		return true;
	}
	return false;
}

bool AItemPickupContainer::OnPickupAddedToInventory(const FPickupItemHandle& PickupItemHandle, const APlayerController* PickupInstigator)
{
	for (auto& AddItemResult : PickupItemHandle.AddItemResults)
	{
		for (auto PickupItemEntryIt = Pickups.Items.CreateIterator(); PickupItemEntryIt; ++PickupItemEntryIt)
		{
			FPickupItemEntry& PickupItemEntry = *PickupItemEntryIt;
			if (AddItemResult.Key == PickupItemEntry.InstanceId)
			{
				if (AddItemResult.Value.Result == EAddItemResult::Success)
				{
					InventoryManager->RemoveInventoryItem(PickupItemEntry.ItemInstanceRef.Get());
					PickupItemEntryIt.RemoveCurrent();
				}
			}
		}
	}
	
	FlushNetDormancy();

	return TryDestroyPickupable();
}
