// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPickupable.h"
#include "PickupItemCollection.h"
#include "SurvivalGame/Interactions/InteractableActor.h"
#include "ItemPickupContainer.generated.h"

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API AItemPickupContainer : public AInteractableActor, public IPickupable
{
	GENERATED_BODY()

public:
	AItemPickupContainer(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// ~Start IPickupable
	virtual TArray<FPickupItemEntry> GetPickupItems() const override;
	virtual bool OnPickupAddedToInventory(const TMap<FGuid, FAddInventoryItemResult> PickupResultMap, const APlayerController* PickupInstigator) override;
	// ~End IPickupable

	UFUNCTION(BlueprintCallable)
	void SetPickupItems(const TArray<FPickupItemEntry> Items);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Pickup")
	bool bDestroyOnPickup = true;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Replicated, Category = "Item Pickup", meta = (ExposeOnSpawn))
	FPickupItemCollection Pickups;

};
