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

	
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category = "Inventory")
	TSubclassOf<UItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Inventory", meta=(DeterminesOutputType=FragmentClass))
	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;
	
private:
	friend struct FInventoryItemList;
	
	UPROPERTY(Replicated)
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY()
	int32 ItemSlot;
	
};
