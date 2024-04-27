#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_Inventory.generated.h"

UCLASS(BlueprintType, meta=(DisplayName="Inventory"))
class UItemFragment_Inventory : public UItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStack = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	bool bCanStack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSoftObjectPtr<UTexture2D> Icon;
};
