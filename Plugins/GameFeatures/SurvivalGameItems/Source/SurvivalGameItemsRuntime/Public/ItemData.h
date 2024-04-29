// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

class UItemDefinition;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipment	UMETA(DisplayText="Equipment"),
	Tool		UMETA(DisplayText="Tool"),
	Material	UMETA(DisplayText="Crafting Material"),
	Cosmetic	UMETA(DisplayText="Cosmetic"),
	Consumable	UMETA(DisplayText="Consumable"),
	Blueprint	UMETA(DisplayText="Blueprint"),
	Currency	UMETA(DisplayText="Currency"),
	Collectible	UMETA(DisplayText="Collectible"),
	QuestItem	UMETA(DisplayText="QuestItem")
};

UCLASS(BlueprintType, Const, meta=(DisplayName = "Item Data"))
class SURVIVALGAMEITEMSRUNTIME_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UItemDefinition> ItemDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMesh> ItemMesh;
};


USTRUCT(BlueprintType)
struct FItemDataDefinition : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName ItemID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSoftClassPtr<UItemDefinition> ItemDefinition;
};

