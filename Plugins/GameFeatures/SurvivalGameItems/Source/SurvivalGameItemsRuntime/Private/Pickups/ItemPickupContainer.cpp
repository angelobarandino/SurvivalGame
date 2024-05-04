// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ItemPickupContainer.h"

#include "InventorySystem/InventoryManagerComponent.h"
#include "InventorySystem/InventoryTypes.h"
#include "Net/UnrealNetwork.h"


AItemPickupContainer::AItemPickupContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	NetUpdateFrequency = 5.f;
	NetDormancy = DORM_Initial;

	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManagerComponent");
}

void AItemPickupContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NetGUID);
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
		NetGUID = FGuid::NewGuid();
		
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
