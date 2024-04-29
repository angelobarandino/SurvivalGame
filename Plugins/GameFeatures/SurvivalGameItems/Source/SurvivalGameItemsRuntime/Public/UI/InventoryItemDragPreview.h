// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventoryItemDragPreview.generated.h"

class UInventoryItemInstance;
/**
 * 
 */
UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UInventoryItemDragPreview : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<const UInventoryItemInstance> ItemInstance;

	virtual void NativeDestruct() override;
	
};
