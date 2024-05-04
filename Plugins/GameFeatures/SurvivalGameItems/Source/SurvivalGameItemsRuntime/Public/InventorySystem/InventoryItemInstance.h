#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInstance.generated.h"

class UItemFragment;
class UItemDefinition;

UCLASS(BlueprintType)
class UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	//~Start of UObject
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TSubclassOf<UItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetItemSlot() const { return ItemSlot; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetItemCount() const { return ItemCount; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Inventory", meta=(DeterminesOutputType=FragmentClass))
	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;
	
private:
	friend struct FInventoryItemList;
	friend class UInventoryManagerComponent;
	friend class UInventoryItemInstanceStatics;

	UPROPERTY(Replicated)
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(Replicated)
	int32 ItemCount;

	UPROPERTY(Replicated)
	int32 ItemSlot;
	
};


UCLASS()
class UInventoryItemInstanceStatics : public UBlueprintFunctionLibrary	
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Item")
	static UInventoryItemInstance* MakeInventoryItemInstance(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);
};
