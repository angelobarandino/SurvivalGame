// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PawnItemManagerComponent.h"

#include "Net/UnrealNetwork.h"


/**
 * //////////////////////////////////////////////////////////////////////////////////////////////////
 * UPawnItemManagerComponent
 */
UPawnItemManagerComponent::UPawnItemManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UPawnItemManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, FocusedItem);
}

void UPawnItemManagerComponent::SetFocusedInventoryItem(const int32 ItemSlot, const bool bIsPlayer)
{
	Server_SetFocusedInventoryItem(ItemSlot, bIsPlayer);
}

void UPawnItemManagerComponent::Server_SetFocusedInventoryItem_Implementation(const int32 ItemSlot, const bool bIsPlayer)
{
	FocusedItem.ItemSlot = ItemSlot;
	FocusedItem.bIsPlayer = bIsPlayer;
}

/**
 * //////////////////////////////////////////////////////////////////////////////////////////////////
 * UPawnItemManagerStatics
 */
void UPawnItemManagerStatics::SetFocusedInventoryItem(const APawn* Pawn, const int32 ItemSlot, const bool bIsPlayer)
{
	if (Pawn == nullptr)
	{
		return;
	}
	
	if (UPawnItemManagerComponent* PawnItemManager = Pawn->FindComponentByClass<UPawnItemManagerComponent>())
	{
		PawnItemManager->SetFocusedInventoryItem(ItemSlot, bIsPlayer);
	}
}

FInventoryFocusedItem UPawnItemManagerStatics::GetFocusedInventoryItem(const APawn* Pawn)
{
	if (Pawn)
	{
		if (const UPawnItemManagerComponent* PawnItemManager = Pawn->FindComponentByClass<UPawnItemManagerComponent>())
		{
			return PawnItemManager->GetFocusedInventoryItem();
		}
	}
	
	return FInventoryFocusedItem();
}
