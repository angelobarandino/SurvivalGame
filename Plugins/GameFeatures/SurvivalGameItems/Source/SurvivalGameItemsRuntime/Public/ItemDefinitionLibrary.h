// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemDefinitionLibrary.generated.h"

class UItemDefinition;
class UItemFragment;
/**
 * 
 */
UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UItemDefinitionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UItemFragment* FindItemDefinitionFragment(TSubclassOf<UItemDefinition> ItemDef, TSubclassOf<UItemFragment> FragmentClass);

	template<typename ItemFragmentClass>
	static const ItemFragmentClass* FindItemDefinitionFragment(TSubclassOf<UItemDefinition> ItemDef)
	{
		return Cast<ItemFragmentClass>(FindItemDefinitionFragment(ItemDef, ItemFragmentClass::StaticClass()));
	}
};
