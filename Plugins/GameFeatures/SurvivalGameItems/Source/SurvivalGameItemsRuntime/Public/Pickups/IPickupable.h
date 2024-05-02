
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickupable.generated.h"

class AItemPickup;
class UInventoryItemInstance;
class UItemDefinition;
struct FAddInventoryItemResult;
struct FPickupItemEntry;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

class SURVIVALGAMEITEMSRUNTIME_API IPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual TArray<FPickupItemEntry> GetPickupItems() const = 0;

	UFUNCTION()
	virtual bool OnPickupAddedToInventory(const TMap<FGuid, FAddInventoryItemResult> PickupResultMap, const APlayerController* PickupInstigator) = 0;
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
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static bool AddAllItemPickupToInventory(APlayerController* PlayerController, TScriptInterface<IPickupable> Pickup);
	
};

