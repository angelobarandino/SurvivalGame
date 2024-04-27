// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "UObject/Interface.h"
#include "IPickupable.generated.h"


class UItemDefinition;

USTRUCT(BlueprintType)
struct FPickupInstance
{
	GENERATED_BODY()

	FPickupInstance() : InstanceId(FGuid::NewGuid()) {}
	FPickupInstance(const TSubclassOf<UItemDefinition> NewItemDef, const int32 NewItemStack)
		: InstanceId(FGuid::NewGuid())
		, ItemDef(NewItemDef)
		, ItemStack(NewItemStack)
	{}

	UPROPERTY(BlueprintReadOnly)
	FGuid InstanceId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemDefinition> ItemDef = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemStack = 1;
};

USTRUCT(BlueprintType)
struct FInventoryPickup
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FPickupInstance> Instances;
};

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
	virtual FInventoryPickup GetPickupInventory() const;
	
	virtual void OnPickupAddedToInventory(const FPickupInstance& PickupInstance, const FAddInventoryItemResult& AddItemResult) = 0;

};

UCLASS()
class UPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TScriptInterface<IPickupable> GetPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(APlayerController* PlayerController, TScriptInterface<IPickupable> Pickup);
	
};

