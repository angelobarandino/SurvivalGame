// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "InteractOptionList.generated.h"

struct FInteractOption;
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInteractOptionList : public UVerticalBox
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BuildInteractOptions(const TArray<FInteractOption>& InteractOptions);

private:
	UWidget* CreateOption(const FInteractOption& Option);
};

