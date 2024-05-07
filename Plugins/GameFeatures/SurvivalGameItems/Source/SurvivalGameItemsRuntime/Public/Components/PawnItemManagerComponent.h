// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnItemManagerComponent.generated.h"


class UInventoryItemInstance;


/**
 * //////////////////////////////////////////////////////////////////////////////////////////////////
 * FInventoryFocusedItem
 */
USTRUCT(BlueprintType)
struct FInventoryFocusedItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 ItemSlot = -1;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayer = false;

	bool HasValidFocused() const
	{
		return ItemSlot > -1;
	}
};


/**
 * //////////////////////////////////////////////////////////////////////////////////////////////////
 * UPawnItemManagerComponent
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAMEITEMSRUNTIME_API UPawnItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPawnItemManagerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetFocusedInventoryItem(const int32 ItemSlot, const bool bIsPlayer);

	FInventoryFocusedItem GetFocusedInventoryItem() const
	{
		return FocusedItem;
	}
	
private:
	UPROPERTY(Replicated)
	FInventoryFocusedItem FocusedItem;
	
	UFUNCTION(Server, Reliable)
	void Server_SetFocusedInventoryItem(const int32 ItemSlot, const bool bIsPlayer);
	
};


/**
 * //////////////////////////////////////////////////////////////////////////////////////////////////
 * UPawnItemManagerStatics
 */
UCLASS()
class UPawnItemManagerStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "PawnItemManagerStatics")
	static void SetFocusedInventoryItem(const APawn* Pawn, const int32 ItemSlot, const bool bIsPlayer);

	UFUNCTION(BlueprintCallable, Category = "PawnItemManagerStatics")
	static FInventoryFocusedItem GetFocusedInventoryItem(const APawn* Pawn);
};
