#include "InventorySystem/InventoryItemInstance.h"

#include "ItemDefinition.h"
#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, ItemCount);
	DOREPLIFETIME(ThisClass, ItemSlot);
}

const UItemFragment* UInventoryItemInstance::FindFragmentByClass(const TSubclassOf<UItemFragment> FragmentClass) const
{
	return UItemDefinitionLibrary::FindItemDefinitionFragment(ItemDef, FragmentClass);
}

UInventoryItemInstance* UInventoryItemInstanceStatics::MakeInventoryItemInstance(const TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount)
{
	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
	ItemInstance->ItemDef = ItemDef;
	ItemInstance->ItemCount = ItemCount;
	return ItemInstance;
}
