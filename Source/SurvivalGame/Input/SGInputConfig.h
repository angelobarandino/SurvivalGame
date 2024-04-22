// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SGInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FSGInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<const UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class SURVIVALGAME_API USGInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	USGInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FSGInputAction> NativeInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FSGInputAction> AbilityInputAction;

	UFUNCTION(BlueprintCallable, Category = "SurvivalGame|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogIfNotFound) const;

	UFUNCTION(BlueprintCallable, Category = "SurvivalGame|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogIfNotFound) const;
};
