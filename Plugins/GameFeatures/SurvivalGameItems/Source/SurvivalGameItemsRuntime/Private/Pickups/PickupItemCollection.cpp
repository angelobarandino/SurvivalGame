#include "Pickups/PickupItemCollection.h"

#include "ItemDefinition.h"

void FPickupItemCollection::AddItems(const TArray<FPickupItemEntry>& NewItems)
{
	for (const FPickupItemEntry& Item : NewItems)
	{
		if (Item.ItemDef != nullptr && Item.ItemStack > 0)
		{
			FPickupItemEntry& NewItem = Items.AddDefaulted_GetRef();
			NewItem.ItemStack = Item.ItemStack;
			NewItem.ItemDef = Item.ItemDef;
			MarkItemDirty(NewItem);
		}
	}
}

void FPickupItemCollection::UpdateItemCount(const FGuid EntryId, const int32 NewItemStack)
{
	for (auto PickupIt = Items.CreateIterator(); PickupIt; ++PickupIt)
	{
		FPickupItemEntry& PickupInstance = *PickupIt;
		if (EntryId == PickupInstance.InstanceId)
		{
			PickupInstance.ItemStack = NewItemStack;
			if (PickupInstance.ItemStack == 0)
			{
				PickupIt.RemoveCurrent();
				MarkArrayDirty();
			}
		}
	}
}
