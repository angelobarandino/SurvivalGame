// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemList.h"
#include "InventoryTypes.h"
#include "Components/ActorComponent.h"
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
	TArray<UInventoryItemInstance*> GetItems() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	FAddInventoryItemResult AddInventorItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);

public:
	void GetItemDefInventoryStack(TSubclassOf<UItemDefinition> ItemDef, bool& bCanStack, int32& MaxStack) const;

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
};
