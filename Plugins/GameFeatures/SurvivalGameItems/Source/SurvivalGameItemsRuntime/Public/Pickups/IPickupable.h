
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickups/ItemPickupContainer.h"
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

	virtual void MoveInventorItem(const int32 OldSlot, const int32 NewSlot) {};
	virtual uint32 GetActorNetGUID() const { return -1; }
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

	template<typename TPickupableActor>
	static TPickupableActor* FindActorByNetGUID(UWorld* World, const int32 ActorNetGUID);
};

template <typename TPickupableActor>
TPickupableActor* UPickupableStatics::FindActorByNetGUID(UWorld* World, const int32 ActorNetGUID)
{
	if (!World)
	{
		return nullptr;
	}

	for (TActorIterator<TPickupableActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (TPickupableActor* CurrentActor = *ActorItr)
		{
			if (IPickupable* Pickupable = Cast<IPickupable>(CurrentActor))
			{
				const uint32 CurrentActorNetGUID = Pickupable->GetActorNetGUID();

				if (CurrentActorNetGUID == ActorNetGUID)
				{
					// Return the actor if found
					return CurrentActor;
				}
			}
		}
	}

	// Return nullptr if no actor with the desired NetGUID is found
	return nullptr;
}

