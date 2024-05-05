// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPickupable.h"
#include "PickupItemCollection.h"
#include "SurvivalGame/Interactions/InteractableActor.h"
#include "ItemPickupContainer.generated.h"

class UInventoryManagerComponent;

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API AItemPickupContainer : public AInteractableActor, public IPickupable
{
	GENERATED_BODY()

public:
	AItemPickupContainer(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;

public:
	// ~Start IPickupable
	virtual TArray<FPickupItemEntry> GetPickupItems() override;
	virtual bool OnPickupAddedToInventory(const FPickupItemHandle& PickupItemHandle, const APlayerController* PickupInstigator) override;
	virtual FGuid GetActorNetGUID() const override { return NetGUID; }
	// ~End IPickupable

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Pickup")
	bool bDestroyOnPickup = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Pickup", meta = (ExposeOnSpawn))
	FPickupItemCollection Pickups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (ExposeOnSpawn))
	int32 MaxInventorySize = 15;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> InventoryManager;

	UPROPERTY(Transient, Replicated)
	FGuid NetGUID;
	
};
