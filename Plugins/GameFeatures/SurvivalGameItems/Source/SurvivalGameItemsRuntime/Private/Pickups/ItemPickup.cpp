// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ItemPickup.h"

#include "ItemDefinition.h"
#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Assets.h"
#include "ItemFragment_Interaction.h"
#include "InventorySystem/InventoryTypes.h"
#include "Net/UnrealNetwork.h"
#include "Pickups/PickupItemCollection.h"

AItemPickup::AItemPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ItemPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemPickupMesh");
	ItemPickupMesh->SetCollisionProfileName(FName("Interactable"));
	SetRootComponent(ItemPickupMesh);

	bReplicates = true;
	NetUpdateFrequency = 5.f;
	NetDormancy = DORM_Initial;
	SetReplicatingMovement(true);
}

void AItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Pickups);
}

void AItemPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetPickupObjectMesh();
}

TArray<FPickupItemEntry> AItemPickup::GetPickupItems() const
{
	return Pickups.Items;
}

bool AItemPickup::OnPickupAddedToInventory(const TMap<FGuid, FAddInventoryItemResult> PickupResultMap, const APlayerController* PickupInstigator)
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

void AItemPickup::SetPickupItems(const TArray<FPickupItemEntry> Items)
{
	Pickups.AddItems(Items);
}

void AItemPickup::OnRep_Pickups()
{
	SetPickupObjectMesh();
}

void AItemPickup::SetPickupObjectMesh()
{
	FPickupItemEntry Pickup;
	if (Pickups.Items.Num() > 0)
	{
		Pickup = Pickups.Items[0];	
	}
	
	if (Pickup.ItemDef == nullptr || Pickup.ItemStack == 0)
	{
		return;
	}
	
	if (const UItemFragment_Assets* AssetFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<UItemFragment_Assets>(Pickup.ItemDef))
	{
		ItemPickupMesh->SetStaticMesh(AssetFragment->Mesh.LoadSynchronous());
	}
	
	if (const UItemFragment_Interaction* InteractionFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<UItemFragment_Interaction>(Pickup.ItemDef))
	{
		InteractableObjectName = InteractionFragment->DisplayName;
		InteractOptions = InteractionFragment->InteractOptions;
		InteractWidgetClass = InteractionFragment->InteractWidgetClass;
	}
}