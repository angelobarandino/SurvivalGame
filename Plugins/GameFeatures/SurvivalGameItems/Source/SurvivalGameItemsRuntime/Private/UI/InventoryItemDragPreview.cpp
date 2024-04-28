// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryItemDragPreview.h"

void UInventoryItemDragPreview::NativeDestruct()
{
	Super::NativeDestruct();

	ItemInstance = nullptr;
}
