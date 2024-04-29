// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

class UItemFragment;

UCLASS(Blueprintable, Const, Abstract)
class SURVIVALGAMEITEMSRUNTIME_API UItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Identifier")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Identifier")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identifier", meta=(MultiLine=true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identifier", meta=(MultiLine=true))
	FText Usage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UItemFragment>> Fragments;

	// @TODO: REMOVE! This might not be needed
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = FragmentClass))
	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;

	// @TODO: REMOVE! This might not be needed
	template<class T = UItemFragment>
	const T* FindFragmentByClass() const;
};

template <class T>
const T* UItemDefinition::FindFragmentByClass() const
{
	const UItemFragment* Fragment = FindFragmentByClass(T::StaticClass());
	return Cast<T>(Fragment);
}
