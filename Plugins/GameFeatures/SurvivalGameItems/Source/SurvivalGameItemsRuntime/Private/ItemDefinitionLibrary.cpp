// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinitionLibrary.h"
#include "ItemDefinition.h"
#include "ItemFragment.h"

const UItemFragment* UItemDefinitionLibrary::FindItemDefinitionFragment(const TSubclassOf<UItemDefinition> ItemDef, const TSubclassOf<UItemFragment> FragmentClass)
{
	if (ItemDef != nullptr && FragmentClass != nullptr)
	{
		if (const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemDef))
		{
			return ItemDefinition->FindFragmentByClass(FragmentClass);
		}
	}

	return nullptr;
}
