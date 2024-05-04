// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPickupable.h"
#include "PickupItemCollection.h"
#include "SurvivalGame/Interactions/InteractableActor.h"
#include "ItemPickupContainer.generated.h"

class UInventoryManagerComponent;
class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API AItemPickupContainer : public AInteractableActor, public IPickupable
{
	GENERATED_BODY()

public:
	AItemPickupContainer(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	// ~Start IPickupable
	virtual TArray<FPickupItemEntry> GetPickupItems() const override;
	virtual bool OnPickupAddedToInventory(const TMap<FGuid, FAddInventoryItemResult> PickupResultMap, const APlayerController* PickupInstigator) override;
	virtual void MoveInventorItem(const int32 OldSlot, const int32 NewSlot) override;
	virtual uint32 GetActorNetGUID() const override { return NetGUID; }
	// ~End IPickupable

	UFUNCTION(BlueprintCallable)
	void SetPickupItems(const TArray<FPickupItemEntry> Items);

	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Pickup")
	bool bDestroyOnPickup = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Pickup", meta = (ExposeOnSpawn))
	FPickupItemCollection Pickups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (ExposeOnSpawn))
	int32 MaxInventorySize = 15;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> InventoryManager;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(Transient, Replicated)
	uint32 NetGUID;
	
private:
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Server_MoveInventorItem(const int32 OldSlot, const int32 NewSlot);
};
