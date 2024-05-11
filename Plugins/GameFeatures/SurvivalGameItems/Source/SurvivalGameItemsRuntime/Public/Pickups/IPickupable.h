
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickupable.generated.h"

class AItemPickup;
class UInventoryItemInstance;
class UInventoryManagerComponent;
class UItemDefinition;
struct FAddInventoryItemResult;
struct FPickupItemEntry;
struct FPickupItemHandle;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

class SURVIVALGAMEITEMSRUNTIME_API IPickupable
{
	GENERATED_BODY()

public:
	
	virtual UInventoryManagerComponent* GetInventoryManagerComponent();
	
	UFUNCTION(BlueprintCallable)
	virtual TArray<FPickupItemEntry> GetPickupItems() = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool TryDestroyPickupable() = 0;

	UFUNCTION()
	virtual bool OnPickupAddedToInventory(const FPickupItemHandle& PickupItemHandle, const APlayerController* PickupInstigator) = 0;
};

UCLASS()
class UPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TScriptInterface<IPickupable> GetPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	static void SpawnItemPickupFromItemInstance(const TSubclassOf<AItemPickup> ItemPickupClass, APawn* OwnerPawn, const UInventoryItemInstance* ItemInstance, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	static void AddItemFromPlayerInventory(APlayerController* PlayerController, TScriptInterface<IPickupable> Pickup, const int32 SourceSlotIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static bool AddAllItemPickupToInventory(APlayerController* PlayerController, TScriptInterface<IPickupable> Pickup);

};