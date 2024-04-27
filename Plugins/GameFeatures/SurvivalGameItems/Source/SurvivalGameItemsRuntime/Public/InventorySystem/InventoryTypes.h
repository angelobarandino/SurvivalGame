#pragma once

#include "CoreMinimal.h"
#include "InventoryTypes.generated.h"


class UInventoryItemInstance;
class UItemDefinition;
///////////////////////////////////////////////////////////////////////
/*
 * FAddItemResult
 */
UENUM(BlueprintType)
enum class EAddItemResult : uint8
{
	Partial,
	Success,
	Failed
};

USTRUCT(BlueprintType)
struct FAddInventoryItemResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EAddItemResult Result = EAddItemResult::Failed;

	UPROPERTY(BlueprintReadOnly)
	int32 RemainingStack = 0;

	static FAddInventoryItemResult GenerateResult(const int32 TotalItemToAdd, const int32 RemainingItemsToAdd)
	{
		FAddInventoryItemResult AddResult;
		AddResult.RemainingStack = RemainingItemsToAdd;

		if (RemainingItemsToAdd == 0)
			AddResult.Result = EAddItemResult::Success;	
		else if (TotalItemToAdd == RemainingItemsToAdd)
			AddResult.Result = EAddItemResult::Failed;	
		else if (TotalItemToAdd > RemainingItemsToAdd)
			AddResult.Result = EAddItemResult::Partial;	
		
		return AddResult;
	}
};



///////////////////////////////////////////////////////////////////////
/*
 * FFindItemSlotResult
 */
UENUM()
enum class EFindItemSlotResult : uint8
{
	InventoryFull,
	CreateNewItem,
	ExistingItem
};

USTRUCT()
struct FAddInventoryItemRequest
{
	GENERATED_BODY()

	FAddInventoryItemRequest()
		: Slot(0)
		, SlotMaxStack(1)
		, bSlotCanStack(false)
		, SlotCurrentItems(0)
		, Result(EFindItemSlotResult::InventoryFull)
		, ItemDef(nullptr)
	{}

	UPROPERTY()
	int32 Slot;

	UPROPERTY()
	int32 SlotMaxStack;
	
	UPROPERTY()
	bool bSlotCanStack;
	
	UPROPERTY()
	int32 SlotCurrentItems;

	UPROPERTY()
	EFindItemSlotResult Result;

	UPROPERTY()
	TSubclassOf<UItemDefinition> ItemDef;
};

USTRUCT()
struct FAddItemResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bSuccess = true;
	
	UPROPERTY()
	int32 ItemsAdded = 0;

	UPROPERTY()
	TWeakObjectPtr<UInventoryItemInstance> Instance = nullptr;
};
