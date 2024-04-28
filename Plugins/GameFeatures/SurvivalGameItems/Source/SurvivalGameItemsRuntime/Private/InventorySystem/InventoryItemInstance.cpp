#include "InventorySystem/InventoryItemInstance.h"

#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, ItemCount);
}

const UItemFragment* UInventoryItemInstance::FindFragmentByClass(const TSubclassOf<UItemFragment> FragmentClass) const
{
	return UItemDefinitionLibrary::FindItemDefinitionFragment(ItemDef, FragmentClass);
}
