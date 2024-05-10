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
	NetUpdateFrequency = 15.f;
	MinNetUpdateFrequency = 5.f;
	NetCullDistanceSquared = 1000000;
	NetDormancy = DORM_Initial;
	SetReplicatingMovement(true);
}

void AItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PickupItem);
}

void AItemPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetPickupObjectMesh();
}

TArray<FPickupItemEntry> AItemPickup::GetPickupItems()
{
	TArray<FPickupItemEntry> PickupItems;
	PickupItems.Add(PickupItem);
	return PickupItems;
}

bool AItemPickup::TryDestroyPickupable()
{
	if (PickupItem.ItemStack == 0 && bDestroyOnPickup)
	{
		Destroy(true, true);
		return true;
	}
	
	return false;
}

bool AItemPickup::OnPickupAddedToInventory(const FPickupItemHandle& PickupItemHandle, const APlayerController* PickupInstigator)
{
	for (auto& AddItemResult : PickupItemHandle.AddItemResults)
	{
		if (AddItemResult.Key == PickupItem.InstanceId)
		{
			PickupItem.ItemStack = AddItemResult.Value.RemainingStack;
		}
	}
	
	ForceNetUpdate();

	return TryDestroyPickupable();
}


void AItemPickup::OnRep_Pickups()
{
	SetPickupObjectMesh();
}

void AItemPickup::SetPickupObjectMesh()
{
	if (PickupItem.ItemDef == nullptr || PickupItem.ItemStack == 0)
	{
		return;
	}
	
	if (const UItemFragment_Assets* AssetFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<UItemFragment_Assets>(PickupItem.ItemDef))
	{
		ItemPickupMesh->SetStaticMesh(AssetFragment->Mesh.LoadSynchronous());
	}
	
	if (const UItemFragment_Interaction* InteractionFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<UItemFragment_Interaction>(PickupItem.ItemDef))
	{
		InteractableObjectName = InteractionFragment->DisplayName;
		InteractOptions = InteractionFragment->InteractOptions;
		InteractWidgetClass = InteractionFragment->InteractWidgetClass;
	}
}