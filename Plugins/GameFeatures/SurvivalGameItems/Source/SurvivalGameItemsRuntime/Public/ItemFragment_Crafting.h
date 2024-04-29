#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_Crafting.generated.h"

class UItemDefinition;

USTRUCT(BlueprintType)
struct FCraftIngredient
{
	GENERATED_BODY()

	/*
	 * 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Material")
	TSoftClassPtr<UItemDefinition> Item = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting Material")
	int32 RequiredCount = 1;
};

UCLASS(meta=(DisplayName = "Crafting"))
class UItemFragment_Crafting : public UItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TArray<FCraftIngredient> Ingredients;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	int32 ProducedItems = 1;
	
};