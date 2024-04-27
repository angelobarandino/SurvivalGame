// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

class UItemFragment;

UCLASS(Blueprintable, Const, Abstract)
class SURVIVALGAMEITEMSRUNTIME_API UItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Display")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Display")
	TArray<TObjectPtr<UItemFragment>> Fragments;
	
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = FragmentClass))
	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;

	template<class T = UItemFragment>
	const T* FindFragmentByClass() const;
};

template <class T>
const T* UItemDefinition::FindFragmentByClass() const
{
	const UItemFragment* Fragment = FindFragmentByClass(T::StaticClass());
	return Cast<T>(Fragment);
}
