#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInstance.h"
#include "InventoryTypes.h"
#include "ItemDefinition.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryItemList.generated.h"

class UInventoryManagerComponent;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct FInventoryItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> ItemInstance = nullptr;
	
	UPROPERTY()
	int32 ItemCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObserveItemCount = 0;
	
	UPROPERTY(NotReplicated)
	int32 LastObserveItemSlot = 0;

};

USTRUCT(BlueprintType)
struct FInventoryItemList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FInventoryItemList() : OwnerInventory(nullptr) {}
	FInventoryItemList(UInventoryManagerComponent* InOwnerInventory) : OwnerInventory(InOwnerInventory)
	{}
	
	// ~Start FFastArraySerializer
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryItemEntry, FInventoryItemList>( Items, DeltaParms, *this );
	}
	//~End of FFastArraySerializer contract


public:
	void MarkItemDirtyAndBroadcastChange(FInventoryItemEntry& Entry);
	
	FAddInventoryItemRequest MakeAddItemRequestToSlot(const int32 Slot, const TSubclassOf<UItemDefinition>& ItemDef);
	FAddInventoryItemRequest MakeAddOrNewItemRequest(const TSubclassOf<UItemDefinition>& ItemDef);

	FInventoryItemEntry* GetInventoryItemAtSlot(const int32 Slot);

	FAddItemResult CreateNewItem(const FAddInventoryItemRequest& ItemRequest, const int32 ItemCount);
	
	FAddItemResult AddItemToSlot(const FAddInventoryItemRequest& ItemRequest, const int32 ItemCount);

	int32 CalculateItemsCanAddToStack(const int32 MaxItemStack, const int32 CurrenItems, const int32 ItemsToAdd) const;

	void MoveItemToSlot(FInventoryItemEntry& Entry, const int32 NewSlot);

	bool RemoveItemEntry(const UInventoryItemInstance* ItemInstance);
	void RemoveItemStack(const UInventoryItemInstance* ItemInstance, const int32 RemoveCount);

private:
	friend UInventoryManagerComponent;
	
	UPROPERTY()
	TArray<FInventoryItemEntry>	Items;

	UPROPERTY(NotReplicated)
	UInventoryManagerComponent* OwnerInventory;

	void BroadcastItemsChangeMessage(const FInventoryItemEntry& Entry, const int32 NewItemCount, const int32 NewItemSlot) const;
};

template<>
struct TStructOpsTypeTraits<FInventoryItemList> : public TStructOpsTypeTraitsBase2<FInventoryItemList>
{
	enum  { WithNetDeltaSerializer = true, };
};