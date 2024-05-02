#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PickupItemCollection.generated.h"

class UItemDefinition;

USTRUCT(BlueprintType)
struct FPickupItemEntry : public FFastArraySerializerItem
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
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 Slot = -1;
};

USTRUCT(BlueprintType)
struct FPickupItemCollection : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FPickupItemEntry>	Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPickupItemEntry, FPickupItemCollection>(Items, DeltaParms, *this);
	}

	void AddItems(const TArray<FPickupItemEntry>& NewItems);
	void UpdateItemCount(FGuid EntryId, const int32 NewItemStack);
};

template<>
struct TStructOpsTypeTraits<FPickupItemCollection> : public TStructOpsTypeTraitsBase2<FPickupItemCollection>
{
	enum { WithNetDeltaSerializer = true, };
};
