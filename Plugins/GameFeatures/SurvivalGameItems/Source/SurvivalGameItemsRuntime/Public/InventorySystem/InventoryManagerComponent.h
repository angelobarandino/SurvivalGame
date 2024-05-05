// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemList.h"
#include "InventoryTypes.h"
#include "Components/ActorComponent.h"
#include "UI/InventoryItemSlotWidget.h"
#include "InventoryManagerComponent.generated.h"

class UItemDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAMEITEMSRUNTIME_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetMaxInventorySize() const { return  MaxInventorySize; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<UInventoryItemInstance*> GetItems(const bool bSort = false) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	FAddInventoryItemResult AddInventorItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MoveInventoryItem(const int32 CurrentSlot, const int32 NewSlot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	bool RemoveInventoryItem(const UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryItemInstance* FindItemInstanceInSlot(const int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetFocusedInventoryItemSlot(const int32 FocusedSlot)
	{
		Server_SetFocusedInventoryItemSlot(FocusedSlot);
	}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	UInventoryItemInstance* GetFocusedInventoryItem()
	{
		return FindItemInstanceInSlot(FocusedInventoryItemSlot);
	}

	UFUNCTION(Server, Reliable)
	void Server_AddInventoryItemFromOtherSource(const int32 TargetSlot, const int32 SourceSlot, UInventoryManagerComponent* SourceInventory);
	
public:
	
	UInventoryItemInstance* AddInitialInventoryItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);
	
	void GetItemDefInventoryStack(TSubclassOf<UItemDefinition> ItemDef, bool& bCanStack, int32& MaxStack) const;

	UPROPERTY(Replicated)
	int32 MaxInventorySize;
private:

	UPROPERTY(Replicated)
	FInventoryItemList InventoryList;
	
	UPROPERTY(Replicated)
	int32 FocusedInventoryItemSlot = -1;

	void ReplicateNewItemInstance(UInventoryItemInstance* NewItemInstance);

	UFUNCTION(Server, Reliable)
	void Server_SetFocusedInventoryItemSlot(const int32 FocusedSlot);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveInventorItem(const int32 CurrentSlot, const int32 NewSlot);
	
};
