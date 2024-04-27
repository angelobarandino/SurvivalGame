// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_Assets.generated.h"

UCLASS(BlueprintType, meta=(DisplayName="Assets"))
class SURVIVALGAMEITEMSRUNTIME_API UItemFragment_Assets : public UItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets")
	TSoftObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets")
	TSoftObjectPtr<UTexture2D> Icon;
};
