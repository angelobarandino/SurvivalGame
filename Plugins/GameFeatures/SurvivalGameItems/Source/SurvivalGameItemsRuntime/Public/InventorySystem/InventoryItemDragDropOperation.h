// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemDragDropOperation.generated.h"

class UInventoryItemInstance;
class UInventoryManagerComponent;


UCLASS(BlueprintType)
class SURVIVALGAMEITEMSRUNTIME_API UInventoryItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> SourceInventory;
	
	UPROPERTY(BlueprintReadOnly)
	int32 DraggedItemSlot;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<const UInventoryItemInstance> DraggedItemInstance;
};
