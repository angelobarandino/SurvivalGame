// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPickupable.h"
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

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Pickup")
	FPickupInstance Pickup;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Pickup")
	bool bDestroyOnPickup = true;
	
	// ~Start IPickupable
	virtual FInventoryPickup GetPickupInventory() const override;
	virtual void OnPickupAddedToInventory(const FPickupInstance& PickupInstance, const FAddInventoryItemResult& AddItemResult) override;
	// ~End IPickupable
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemPickupMesh;
	
};
