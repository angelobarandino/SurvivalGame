#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_Inventory.generated.h"

class UInventoryItemTooltip;

UCLASS(BlueprintType, meta=(DisplayName="Inventory"))
class UItemFragment_Inventory : public UItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	bool bCanStack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta=(EditCondition="bCanStack==true"))
	int32 MaxStack = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float Weight = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float Value = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetClass;
	
};
