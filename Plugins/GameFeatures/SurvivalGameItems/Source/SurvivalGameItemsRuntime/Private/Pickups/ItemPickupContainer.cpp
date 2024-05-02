// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ItemPickupContainer.h"

#include "InventorySystem/InventoryManagerComponent.h"
#include "InventorySystem/InventoryTypes.h"
#include "Net/UnrealNetwork.h"


AItemPickupContainer::AItemPickupContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManagerComponent");
	
	bReplicates = true;
	NetUpdateFrequency = 5.f;
	NetDormancy = DORM_Initial;
}

void AItemPickupContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Pickups);
}

void AItemPickupContainer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AItemPickupContainer::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		InventoryManager->MaxInventorySize = MaxInventorySize;

		for (const FPickupItemEntry& PickupItem : Pickups.Items)
		{
			InventoryManager->AddInitialInventoryItem(PickupItem.ItemDef, PickupItem.ItemStack);
		}
		
		ForceNetUpdate();
	}
}

TArray<FPickupItemEntry> AItemPickupContainer::GetPickupItems() const
{
	return Pickups.Items;
}

bool AItemPickupContainer::OnPickupAddedToInventory(const TMap<FGuid, FAddInventoryItemResult> PickupResultMap, const APlayerController* PickupInstigator)
{
	for (const auto& Result : PickupResultMap)
	{
		Pickups.UpdateItemCount(Result.Key, Result.Value.RemainingStack);
	}
	
	FlushNetDormancy();

	const bool bCanBeDestroyed = Pickups.Items.Num() == 0 && bDestroyOnPickup;

	if (bCanBeDestroyed)
	{
		Destroy(true, true);
	}

	return bCanBeDestroyed;
}

void AItemPickupContainer::SetPickupItems(const TArray<FPickupItemEntry> Items)
{
	Pickups.AddItems(Items);
}
