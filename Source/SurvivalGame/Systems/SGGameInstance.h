﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USGGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
};