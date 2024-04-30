// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemList.h"
#include "InventoryTypes.h"
#include "Components/ActorComponent.h"
#include "UI/InventoryItemSlotWidget.h"
#include "InventoryManagerComponent.generated.h"

class UItemDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusedInventoryItemChange, const UInventoryItemInstance*, ItemInstance);

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
	TArray<UInventoryItemInstance*> GetItems() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	FAddInventoryItemResult AddInventorItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);

	UFUNCTION(Blueprintable, Category = "Inventory")
	bool MoveInventorItem(const int32 CurrentSlot, const int32 NewSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveInventoryItem(const UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropInventoryItem(const FDropInventoryItemTemplate& DropItemTemplate);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryItemInstance* FindItemInstanceInSlot(const int32 Slot);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnFocusedInventoryItemChange OnFocusedInventoryItemChange;
	
public:
	void GetItemDefInventoryStack(TSubclassOf<UItemDefinition> ItemDef, bool& bCanStack, int32& MaxStack) const;

	void SetFocusedItem(const UInventoryItemInstance* ItemInstance) const
	{
		OnFocusedInventoryItemChange.Broadcast(ItemInstance);
	}
	
private:

	UPROPERTY(Replicated, ReplicatedUsing=OnRep_InventoryItems)
	FInventoryItemList InventoryList;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_MaxInventorySize)
	int32 MaxInventorySize;

	UFUNCTION()
	void OnRep_InventoryItems();
	
	UFUNCTION()
	void OnRep_MaxInventorySize();

	void ReplicateNewItemInstance(UInventoryItemInstance* NewItemInstance);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveInventorItem(const int32 CurrentSlot, const int32 NewSlot);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveInventoryItem(const UInventoryItemInstance* ItemInstance);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DropInventoryItem(const FDropInventoryItemTemplate& DropItemTemplate);

};
