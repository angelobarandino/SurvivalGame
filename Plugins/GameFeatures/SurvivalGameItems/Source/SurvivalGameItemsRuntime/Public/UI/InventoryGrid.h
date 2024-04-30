// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventoryGrid.generated.h"

class UInventoryItemSlotWidget;
class UInventoryItemInstance;


UCLASS(BlueprintType)
class SURVIVALGAMEITEMSRUNTIME_API UInventoryGrid : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UInventoryGrid(const FObjectInitializer& ObjectInitializer);

};
