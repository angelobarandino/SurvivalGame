
#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "UObject/Interface.h"
#include "IPickupable.generated.h"

class AItemPickup;
class UInventoryItemInstance;
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
	UFUNCTION(BlueprintCallable)
	virtual TArray<FPickupItemEntry> GetPickupItems() = 0;

	UFUNCTION()
	virtual bool OnPickupAddedToInventory(const FPickupItemHandle& PickupItemHandle, const APlayerController* PickupInstigator) = 0;

	virtual FGuid GetActorNetGUID() const { return FGuid(); }
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

	template <class TPickupableActor>
	static AActor* FindActorByNetGUID(UWorld* World, const FGuid ActorNetGUID);
};

template <class TPickupableActor>
AActor* UPickupableStatics::FindActorByNetGUID(UWorld* World, const FGuid ActorNetGUID)
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
				const FGuid CurrentActorNetGUID = Pickupable->GetActorNetGUID();

				if (CurrentActorNetGUID == ActorNetGUID)
				{
					// Return the actor if found
					return Cast<AActor>(CurrentActor);
				}
			}
		}
	}

	// Return nullptr if no actor with the desired NetGUID is found
	return nullptr;
}