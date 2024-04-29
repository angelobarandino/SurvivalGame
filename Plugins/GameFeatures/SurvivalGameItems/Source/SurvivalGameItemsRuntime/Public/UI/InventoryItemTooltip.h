// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventoryItemTooltip.generated.h"

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UInventoryItemTooltip : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	
	FORCEINLINE void SetItemData(const TSubclassOf<UItemDefinition> InItemDefinition, const int32 InItemCount)
	{
		ItemCount = InItemCount;
		ItemDefinition = InItemDefinition;
	}

protected:
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UItemDefinition> ItemDefinition;
	
	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount;
};
