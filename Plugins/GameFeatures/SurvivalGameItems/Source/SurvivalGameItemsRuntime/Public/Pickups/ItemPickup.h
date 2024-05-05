// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPickupable.h"
#include "PickupItemCollection.h"
#include "GameFramework/Actor.h"
#include "SurvivalGame/Interactions/InteractableActor.h"
#include "ItemPickup.generated.h"

class UItemData;


UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API AItemPickup : public AInteractableActor, public IPickupable
{
	GENERATED_BODY()

public:
	AItemPickup(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	void SetPickupItem(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount)
	{
		PickupItem.ItemDef = ItemDef;
		PickupItem.ItemStack = ItemCount;
		SetPickupObjectMesh();
	}

	// ~Start IPickupable
	virtual TArray<FPickupItemEntry> GetPickupItems() override;
	virtual bool OnPickupAddedToInventory(const FPickupItemHandle& PickupItemHandle, const APlayerController* PickupInstigator) override;
	// ~End IPickupable

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Pickup")
	bool bDestroyOnPickup = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_Pickups, Category = "Item Pickup", meta = (ExposeOnSpawn))
	FPickupItemEntry PickupItem;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemPickupMesh;

	UFUNCTION()
	void OnRep_Pickups();

private:
	void SetPickupObjectMesh();
};
