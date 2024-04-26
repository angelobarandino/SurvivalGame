// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGCharacterLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGCharacterLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	static const USkeletalMeshComponent* GetCharacterSkeletalMesh(const AActor* Actor);
	
};
