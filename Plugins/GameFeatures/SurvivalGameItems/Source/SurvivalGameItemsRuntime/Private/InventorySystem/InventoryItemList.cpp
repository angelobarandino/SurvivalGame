#include "InventorySystem/InventoryItemList.h"
#include "NativeGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryManagerComponent.h"


UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_SurvivalGame_Inventory_Message_StackChange, "SurvivalGame.Inventory.Message.StackChange");


void FInventoryItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		FInventoryItemEntry& Entry = Items[Index];
		BroadcastItemsChangeMessage(Entry, 0, 0);
		
		Entry.LastObserveItemCount = 0;
		Entry.LastObserveItemSlot = 0;
	}
}

void FInventoryItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		FInventoryItemEntry& Entry = Items[Index];
		BroadcastItemsChangeMessage(Entry, Entry.ItemCount, Entry.ItemInstance->ItemSlot);
		
		Entry.LastObserveItemCount = Entry.ItemCount;
		Entry.LastObserveItemSlot = Entry.ItemInstance->ItemSlot;
	}
}

void FInventoryItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
		FInventoryItemEntry& Entry = Items[Index];
		BroadcastItemsChangeMessage(Entry, Entry.ItemCount, Entry.ItemInstance->ItemSlot);
		
		Entry.LastObserveItemCount = Entry.ItemCount;
		Entry.LastObserveItemSlot = Entry.ItemInstance->ItemSlot;
	}
}


void FInventoryItemList::BroadcastItemsChangeMessage(const FInventoryItemEntry& Entry, const int32 NewItemCount, const int32 NewItemSlot) const
{
	check(Entry.ItemInstance);
	
	FInventoryChangeMessage Message;
	Message.ItemCount = Entry.ItemCount;
	Message.ItemInstance = Entry.ItemInstance;
	Message.ItemDef = Entry.ItemInstance->ItemDef;
	Message.OldItemSlot = Entry.LastObserveItemSlot;
	Message.OwnerActor = OwnerInventory->GetOwner();
	
	const int32 OldItemCount = Entry.LastObserveItemCount;
	const int32 OldItemSlot = Entry.LastObserveItemSlot;
	
	if (OldItemCount != NewItemCount)
	{
		if (OldItemCount == 0 && NewItemCount > 0)
		{
			Message.ChangeAction = EInventoryChangeAction::ItemAdded;
		}
		else if (OldItemCount > 0 && NewItemCount == 0)
		{
			Message.ChangeAction = EInventoryChangeAction::ItemRemoved;
		}
		else
		{
			Message.ChangeAction = EInventoryChangeAction::ItemStackChanged;
		}
	}
	else if (OldItemSlot != NewItemSlot)
	{
		Message.ChangeAction = EInventoryChangeAction::ItemMovedToSlot;
	}
	
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerInventory->GetWorld());
	MessageSystem.BroadcastMessage(TAG_SurvivalGame_Inventory_Message_StackChange, Message);
}

void FInventoryItemList::MarkItemDirtyAndBroadcastChange(FInventoryItemEntry& Entry)
{
	MarkItemDirty(Entry);

	if (OwnerInventory->GetOwner()->HasAuthority())
	{
		BroadcastItemsChangeMessage(Entry, Entry.ItemCount, Entry.ItemInstance->ItemSlot);
		Entry.LastObserveItemCount = Entry.ItemCount;
		Entry.LastObserveItemSlot = Entry.ItemInstance->ItemSlot;
	}
}


FAddInventoryItemRequest FInventoryItemList::MakeAddItemRequestToSlot(const int32 Slot, const TSubclassOf<UItemDefinition>& ItemDef)
{
	check(ItemDef);
	check(OwnerInventory);
	
	FAddInventoryItemRequest Result;
	Result.ItemDef = ItemDef;
	Result.SlotCurrentItems = 0;
	Result.Slot = 0;
	
	OwnerInventory->GetItemDefInventoryStack(ItemDef, Result.bSlotCanStack, Result.SlotMaxStack);
	
	if (const FInventoryItemEntry* Entry = GetInventoryItemAtSlot(Slot))
	{
		if (Entry->ItemInstance && Entry->ItemInstance->ItemDef == ItemDef)
		{
			if (!Result.bSlotCanStack || Result.SlotMaxStack == Entry->ItemCount)
			{
				Result.Result = EFindItemSlotResult::Invalid;
				return Result;
			}
			
			Result.Result = EFindItemSlotResult::ExistingItem;
			Result.Slot = Entry->ItemInstance->GetItemSlot();
			Result.SlotCurrentItems = Entry->ItemCount;
			return Result;
		}

		// requested slot contains different item
		// so we return an invalid request
		Result.Result = EFindItemSlotResult::Invalid;
		return Result;
	}

	Result.Result = EFindItemSlotResult::InsertNewItem;
	return Result;
}

FAddInventoryItemRequest FInventoryItemList::MakeAddOrNewItemRequest(const TSubclassOf<UItemDefinition>& ItemDef)
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
		Result.Result = EFindItemSlotResult::InsertNewItem;
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

	const int32 ItemsToAdd = CalculateItemsCanAddToStack(ItemRequest.SlotMaxStack, ItemRequest.SlotCurrentItems, ItemCount);

	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>(OwningActor);
	ItemInstance->ItemCount = ItemsToAdd;
	ItemInstance->ItemDef = ItemRequest.ItemDef;
	ItemInstance->ItemSlot = ItemRequest.Slot;

	FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
	NewItem.ItemInstance = ItemInstance;
	NewItem.ItemCount = ItemsToAdd;
	
	MarkItemDirtyAndBroadcastChange(NewItem);
	
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
				Entry->ItemInstance->ItemCount = Entry->ItemCount;
				
				Result.bSuccess = true;
				Result.ItemsAdded = ItemsToAdd;
				Result.Instance = ItemInstance;
				
				MarkItemDirtyAndBroadcastChange(*Entry);
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

void FInventoryItemList::MoveItemToSlot(FInventoryItemEntry& Entry, const int32 NewSlot)
{
	Entry.ItemInstance->ItemSlot = NewSlot;
	MarkItemDirtyAndBroadcastChange(Entry);
	Entry.LastObserveItemSlot = NewSlot;
}

bool FInventoryItemList::RemoveItemEntry(const UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		for (auto EntryIt = Items.CreateIterator(); EntryIt; ++EntryIt)
		{
			const FInventoryItemEntry& Entry = *EntryIt;
			if (Entry.ItemInstance == ItemInstance)
			{
				if (OwnerInventory->GetOwner()->HasAuthority())
				{
					BroadcastItemsChangeMessage(Entry, 0, 0);
				}
			
				EntryIt.RemoveCurrent();
				MarkArrayDirty();
				return true;
			}
		}
	}

	return false;
}

void FInventoryItemList::RemoveItemStack(const UInventoryItemInstance* ItemInstance, const int32 RemoveCount)
{
	for (auto EntryIt = Items.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInventoryItemEntry& Entry = *EntryIt;
		if (Entry.ItemInstance == ItemInstance)
		{
			Entry.ItemCount -= RemoveCount;
			Entry.ItemInstance->ItemCount = Entry.ItemCount;
			check(Entry.ItemCount >= 0);

			// if new ItemCount is zero, delete item from inventory list
			if (Entry.ItemCount == 0)
			{
				if (OwnerInventory->GetOwner()->HasAuthority())
				{
					BroadcastItemsChangeMessage(Entry, 0, 0);
				}
				
				EntryIt.RemoveCurrent();
				MarkArrayDirty();
				break;
			}

			MarkItemDirtyAndBroadcastChange(Entry);
			break;
		}
	}
}
