// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinition.h"

#include "ItemFragment.h"

const UItemFragment* UItemDefinition::FindFragmentByClass(const TSubclassOf<UItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (const UItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
