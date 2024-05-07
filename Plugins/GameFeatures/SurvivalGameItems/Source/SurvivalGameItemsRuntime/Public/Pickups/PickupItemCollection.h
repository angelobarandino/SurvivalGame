#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "PickupItemCollection.generated.h"

class UItemDefinition;

USTRUCT(BlueprintType)
struct FPickupItemEntry
{
	GENERATED_BODY()
	
	FPickupItemEntry() : InstanceId(FGuid::NewGuid()) {}
	FPickupItemEntry(const TSubclassOf<UItemDefinition> NewItemDef, const int32 NewItemStack = 1)
		: InstanceId(FGuid::NewGuid())
		, ItemDef(NewItemDef)
		, ItemStack(NewItemStack)
	{}

	UPROPERTY(BlueprintReadOnly)
	FGuid InstanceId;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<UItemDefinition> ItemDef = nullptr;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 ItemStack = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemSlot = -1;
	
	TWeakObjectPtr<const UInventoryItemInstance> ItemInstanceRef;
};

USTRUCT(BlueprintType)
struct FPickupItemCollection
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FPickupItemEntry> Items;
};

USTRUCT()
struct FPickupItemHandle
{
	GENERATED_BODY()

	TMap<FGuid, FAddInventoryItemResult> AddItemResults;

	void AddResult(const FGuid& InstanceId, const FAddInventoryItemResult& Result)
	{
		AddItemResults.Add(InstanceId, Result);
	}
};