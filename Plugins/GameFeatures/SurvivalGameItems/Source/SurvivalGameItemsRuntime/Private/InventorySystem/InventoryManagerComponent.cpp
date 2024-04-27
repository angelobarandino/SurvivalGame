// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryManagerComponent.h"

#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "Engine/ActorChannel.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryItemList.h"
#include "Net/UnrealNetwork.h"

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

TArray<UInventoryItemInstance*> UInventoryManagerComponent::GetItems() const
{
	TArray<UInventoryItemInstance*> Items;
	Items.Reserve(InventoryList.Items.Num());

	for (int Index = 0; Index < InventoryList.Items.Num(); ++Index)
	{
		Items.Add(InventoryList.Items[Index].ItemInstance);
	}

	return Items;
}

FAddInventoryItemResult UInventoryManagerComponent::AddInventorItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount)
{
	check(ItemDef);

	int32 RemainingItemCount = ItemCount;
	while (RemainingItemCount > 0)
	{
		const FAddInventoryItemRequest& AddItemRequest = InventoryList.MakeAddItemRequest(ItemDef);

		if (AddItemRequest.Result == EFindItemSlotResult::CreateNewItem)
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

void UInventoryManagerComponent::GetItemDefInventoryStack(const TSubclassOf<UItemDefinition> ItemDef, bool& bCanStack, int32& MaxStack) const
{
	check(ItemDef);
	if (const UItemFragment_Inventory* InventoryFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<UItemFragment_Inventory>(ItemDef))
	{
		bCanStack = InventoryFragment->bCanStack;
		MaxStack = FMath::Clamp(InventoryFragment->MaxStack, 1, InventoryFragment->MaxStack);
	}
}

void UInventoryManagerComponent::OnRep_InventoryItems()
{
	
}

void UInventoryManagerComponent::OnRep_MaxInventorySize()
{
	
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

