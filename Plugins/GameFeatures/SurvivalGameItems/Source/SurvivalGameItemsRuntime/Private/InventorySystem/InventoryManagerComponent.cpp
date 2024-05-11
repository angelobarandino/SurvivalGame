// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryManagerComponent.h"

#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "Engine/ActorChannel.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryItemList.h"
#include "Net/UnrealNetwork.h"
#include "Pickups/ItemPickup.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryManagerComponent)

UInventoryManagerComponent::UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	MaxInventorySize = 56;
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, MaxInventorySize);
	DOREPLIFETIME(ThisClass, FocusedInventoryItemSlot);
}

bool UInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FInventoryItemEntry& Entry : InventoryList.Items)
	{
		if (IsValid(Entry.ItemInstance))
		{
			Result |= Channel->ReplicateSubobject(Entry.ItemInstance, *Bunch, *RepFlags);
		}
	}
	
	return Result;
}

void UInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryItemEntry& Entry : InventoryList.Items)
		{
			if (IsValid(Entry.ItemInstance))
			{
				AddReplicatedSubObject(Entry.ItemInstance);
			}
		}
	}
}

TArray<UInventoryItemInstance*> UInventoryManagerComponent::GetItems(const bool bSort) const
{
	TArray<UInventoryItemInstance*> Items;
	Items.Reserve(InventoryList.Items.Num());

	for (int Index = 0; Index < InventoryList.Items.Num(); ++Index)
	{
		Items.Add(InventoryList.Items[Index].ItemInstance);
	}

	if (bSort)
	{
		Items.Sort([](const UInventoryItemInstance& A, const UInventoryItemInstance& B)
		{
			return A.ItemSlot < B.ItemSlot;
		});
	}
	
	return Items;
}

FAddInventoryItemResult UInventoryManagerComponent::AddInventorItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount)
{
	check(ItemDef && ItemCount > 0);

	int32 RemainingItemCount = ItemCount;
	while (RemainingItemCount > 0)
	{
		const FAddInventoryItemRequest& AddItemRequest = InventoryList.MakeAddOrNewItemRequest(ItemDef);

		if (AddItemRequest.Result == EFindItemSlotResult::InsertNewItem)
		{
			FAddItemResult AddItemResult = InventoryList.CreateNewItem(AddItemRequest, RemainingItemCount);
			RemainingItemCount -= AddItemResult.ItemsAdded;
			ReplicateNewItemInstance(AddItemResult.Instance.Get());
		}
		else if (AddItemRequest.Result == EFindItemSlotResult::ExistingItem)
		{
			const FAddItemResult AddItemResult = InventoryList.AddItemToSlot(AddItemRequest, RemainingItemCount);
			RemainingItemCount -= AddItemResult.ItemsAdded;
		}
		else
		{
			break;
		}
	}

	return FAddInventoryItemResult::GenerateResult(ItemCount, RemainingItemCount);
}

bool UInventoryManagerComponent::MoveInventoryItem(const int32 CurrentSlot, const int32 NewSlot)
{
	Server_MoveInventorItem(CurrentSlot, NewSlot);
	return true;
}

bool UInventoryManagerComponent::RemoveInventoryItem(const UInventoryItemInstance* ItemInstance)
{
	return InventoryList.RemoveItemEntry(ItemInstance);
}

UInventoryItemInstance* UInventoryManagerComponent::FindItemInstanceInSlot(const int32 Slot)
{
	if (const FInventoryItemEntry* Entry = InventoryList.GetInventoryItemAtSlot(Slot))
	{
		return Entry->ItemInstance;
	}

	return nullptr;
}

int32 UInventoryManagerComponent::FindAvailableSlot()
{
	for (int SlotIndex = 0; SlotIndex < MaxInventorySize; ++SlotIndex)
	{
		if (InventoryList.GetInventoryItemAtSlot(SlotIndex))
		{
			continue;
		}

		return SlotIndex;
	}

	return -1;
}

bool UInventoryManagerComponent::Server_AddInventoryItemFromOtherSource(const int32 SourceSlot, UInventoryManagerComponent* SourceInventory)
{
	if (SourceInventory)
	{
		if (const UInventoryItemInstance* SourceItemInstance = SourceInventory->FindItemInstanceInSlot(SourceSlot))
		{
			const int32 SourceItemCount = SourceItemInstance->GetItemCount();
			const FAddInventoryItemResult& AddItemResult = AddInventorItem(
				SourceItemInstance->GetItemDef(), SourceItemCount);
			
			if (AddItemResult.Result != EAddItemResult::Failed)
			{
				const int32 ItemsAdded = SourceItemCount - AddItemResult.RemainingStack;
				
				SourceInventory->InventoryList.RemoveItemStack(SourceItemInstance, ItemsAdded);
				
				return true;
			}
		}
	}
	
	return false;
}

bool UInventoryManagerComponent::Server_AddInventoryItemFromOtherSourceWithTargetSlot(const int32 TargetSlot, const int32 SourceSlot, UInventoryManagerComponent* SourceInventory)
{
	if (SourceInventory)
	{
		if (const UInventoryItemInstance* SourceItemInstance = SourceInventory->FindItemInstanceInSlot(SourceSlot))
		{
			const FAddInventoryItemRequest& Request = InventoryList.MakeAddItemRequestToSlot(TargetSlot, SourceItemInstance->GetItemDef());
			if (Request.Result == EFindItemSlotResult::Invalid)
			{
				return false;
			}

			FAddItemResult Result;
			if (Request.Result == EFindItemSlotResult::ExistingItem)
			{
				Result = InventoryList.AddItemToSlot(Request, SourceItemInstance->GetItemCount());
			}
			else if (Request.Result == EFindItemSlotResult::InsertNewItem)
			{
				Result = InventoryList.CreateNewItem(Request, SourceItemInstance->GetItemCount());
				if (Result.bSuccess)
				{
					ReplicateNewItemInstance(Result.Instance.Get());
				}
			}

			if (Result.bSuccess)
			{
				SourceInventory->InventoryList.RemoveItemStack(SourceItemInstance, Result.ItemsAdded);
				return true;
			}
		}
	}

	return false;
}

bool UInventoryManagerComponent::Server_MoveInventorItem(const int32 CurrentSlot, const int32 NewSlot)
{
	if (CurrentSlot == NewSlot)
	{
		return false;
	}
	
	if (FInventoryItemEntry* CurrentEntry = InventoryList.GetInventoryItemAtSlot(CurrentSlot))
	{
		if (const UInventoryItemInstance* CurrentItemInstance = CurrentEntry->ItemInstance)
		{
			const FAddInventoryItemRequest& MoveRequest = InventoryList.MakeAddItemRequestToSlot(NewSlot, CurrentItemInstance->GetItemDef());
			
			if (MoveRequest.Result == EFindItemSlotResult::ExistingItem)
			{
				const FAddItemResult ItemResult = InventoryList.AddItemToSlot(MoveRequest, CurrentEntry->ItemCount);
				if (ItemResult.bSuccess)
				{
					InventoryList.RemoveItemStack(CurrentEntry->ItemInstance, ItemResult.ItemsAdded);
					return true;
				}
			}
			else if (MoveRequest.Result == EFindItemSlotResult::InsertNewItem)
			{
				InventoryList.MoveItemToSlot(*CurrentEntry, NewSlot);
				return true;
			}
		}
	}
	
	return false;
}

UInventoryItemInstance* UInventoryManagerComponent::AddInitialInventoryItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount)
{
	FAddItemResult Result = InventoryList.CreateNewItem(InventoryList.MakeAddOrNewItemRequest(ItemDef), ItemCount);
	if (Result.bSuccess)
	{
		UInventoryItemInstance* ItemInstance = Result.Instance.Get();
		ReplicateNewItemInstance(ItemInstance);
		return ItemInstance;
	}
	return nullptr;
}

void UInventoryManagerComponent::GetItemDefInventoryStack(const TSubclassOf<UItemDefinition> ItemDef, bool& bCanStack, int32& MaxStack) const
{
	check(ItemDef);
	if (const UItemFragment_Inventory* InventoryFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<UItemFragment_Inventory>(ItemDef))
	{
		bCanStack = InventoryFragment->bCanStack;
		MaxStack = FMath::Clamp(InventoryFragment->MaxStack, 1, InventoryFragment->MaxStack);
	}
}

void UInventoryManagerComponent::ReplicateNewItemInstance(UInventoryItemInstance* NewItemInstance)
{
	if (NewItemInstance)
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(NewItemInstance);
		}
	}
}

void UInventoryManagerComponent::Server_SetFocusedInventoryItemSlot_Implementation(const int32 FocusedSlot)
{
	FocusedInventoryItemSlot = FocusedSlot;
}
