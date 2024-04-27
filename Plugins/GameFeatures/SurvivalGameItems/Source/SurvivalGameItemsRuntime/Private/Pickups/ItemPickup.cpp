// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ItemPickup.h"

#include "ItemDefinition.h"
#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Assets.h"
#include "ItemFragment_Interaction.h"

AItemPickup::AItemPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ItemPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemPickupMesh");
	ItemPickupMesh->SetCollisionProfileName(FName("Interactable"));
	SetRootComponent(ItemPickupMesh);

	bReplicates = true;
}

void AItemPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
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

FInventoryPickup AItemPickup::GetPickupInventory() const
{
	if (Pickup.ItemDef == nullptr)
	{
		return FInventoryPickup();
	}
	
	FInventoryPickup InventoryPickup;
	InventoryPickup.Instances.Add(Pickup);
	return InventoryPickup;
}

void AItemPickup::OnPickupAddedToInventory(const FPickupInstance& PickupInstance, const FAddInventoryItemResult& AddItemResult)
{
	Pickup.ItemStack = AddItemResult.RemainingStack;
	
	if (bDestroyOnPickup && Pickup.ItemStack == 0)
	{
		Destroy();
	}
}
