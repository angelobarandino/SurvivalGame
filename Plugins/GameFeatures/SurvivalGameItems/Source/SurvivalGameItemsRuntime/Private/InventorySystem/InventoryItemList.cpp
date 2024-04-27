#include "InventorySystem/InventoryItemList.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryManagerComponent.h"


void FInventoryItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FInventoryItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FInventoryItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

FAddInventoryItemRequest FInventoryItemList::MakeAddItemRequest(const TSubclassOf<UItemDefinition>& ItemDef)
{
	check(ItemDef);
	check(OwnerInventory);
	
	FAddInventoryItemRequest Result;
	Result.ItemDef = ItemDef;
	Result.SlotCurrentItems = 0;
	Result.Slot = 0;
	
	const int MaxInventoryItems = OwnerInventory->GetMaxInventorySize();
	const int CurrentInventoryItems = Items.Num();

	// Inventory full
	if (MaxInventoryItems == CurrentInventoryItems)
	{
		Result.Result = EFindItemSlotResult::InventoryFull;
		return Result;
	}

	int32 MaxStack = 1;
	bool bCanStack = false;
	OwnerInventory->GetItemDefInventoryStack(ItemDef, bCanStack, MaxStack);
	
	for (int InventorySlot = 0; InventorySlot < MaxInventoryItems; ++InventorySlot)
	{
		if(const FInventoryItemEntry* Entry = GetInventoryItemAtSlot(InventorySlot))
		{
			const UInventoryItemInstance* ItemInstance = Entry->ItemInstance;
			
			if (ItemInstance && ItemInstance->ItemDef)
			{
				// if the found entry is the same with the item we are looking for
				// return it as the result, but only if we can add more to its stack.
				if (ItemInstance->ItemDef == ItemDef)
				{
					if (!bCanStack || MaxStack == Entry->ItemCount)
					{
						continue;
					}
				
					Result.Slot = ItemInstance->ItemSlot;
					Result.SlotCurrentItems = Entry->ItemCount;
					Result.Result = EFindItemSlotResult::ExistingItem;
					break;
				}

				// continue searching.
				// current slot is already assigned with a different item.
				continue;
			}
		}
		
		Result.Slot = InventorySlot;
		Result.Result = EFindItemSlotResult::CreateNewItem;
		break;
	}

	Result.SlotMaxStack = MaxStack;
	Result.bSlotCanStack = bCanStack;
	return Result;
}

FInventoryItemEntry* FInventoryItemList::GetInventoryItemAtSlot(const int32 Slot)
{
	for (auto EntryIt = Items.CreateIterator(); EntryIt; ++ EntryIt)
	{
		FInventoryItemEntry& Entry = *EntryIt;
		if (Entry.ItemInstance && Entry.ItemInstance->ItemSlot == Slot)
		{
			return &Entry;
		}
	}

	return nullptr;
}

FAddItemResult FInventoryItemList::CreateNewItem(const FAddInventoryItemRequest& ItemRequest, const int32 ItemCount)
{
	check(OwnerInventory);
	check(ItemRequest.ItemDef);

	AActor* OwningActor = OwnerInventory->GetOwner();
	check(OwningActor->HasAuthority());

	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>(OwningActor);
	ItemInstance->ItemDef = ItemRequest.ItemDef;
	ItemInstance->ItemSlot = ItemRequest.Slot;

	const int32 ItemsToAdd = CalculateItemsCanAddToStack(ItemRequest.SlotMaxStack, ItemRequest.SlotCurrentItems, ItemCount);
	
	FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
	NewItem.ItemInstance = ItemInstance;
	NewItem.ItemCount = ItemsToAdd;
	MarkItemDirty(NewItem);

	FAddItemResult Result;
	Result.bSuccess = true;
	Result.Instance = ItemInstance;
	Result.ItemsAdded = ItemsToAdd;
	return Result;
}

FAddItemResult FInventoryItemList::AddItemToSlot(const FAddInventoryItemRequest& ItemRequest, const int32 ItemCount)
{
	FAddItemResult Result;

	if (FInventoryItemEntry* Entry = GetInventoryItemAtSlot(ItemRequest.Slot))
	{
		UInventoryItemInstance* ItemInstance = Entry->ItemInstance;
		check(ItemInstance);
		
		if (ItemInstance->ItemDef == ItemRequest.ItemDef)
		{
			const int32 ItemsToAdd = CalculateItemsCanAddToStack(ItemRequest.SlotMaxStack, ItemRequest.SlotCurrentItems, ItemCount);
			
			if (ItemsToAdd > 0)
			{
				Entry->ItemCount += ItemsToAdd;
				
				Result.bSuccess = true;
				Result.ItemsAdded = ItemsToAdd;
				Result.Instance = ItemInstance;
				MarkItemDirty(*Entry);
			}
		}
	}

	return Result;
}

int32 FInventoryItemList::CalculateItemsCanAddToStack(const int32 MaxItemStack, const int32 CurrenItems, const int32 ItemsToAdd) const
{
	const int32 ItemsCanAddToStack = MaxItemStack - CurrenItems;

	if (ItemsToAdd > ItemsCanAddToStack)
	{
		return ItemsCanAddToStack;
	}

	if (ItemsToAdd < ItemsCanAddToStack)
	{
		return ItemsToAdd;
	}

	return ItemsCanAddToStack;
}
