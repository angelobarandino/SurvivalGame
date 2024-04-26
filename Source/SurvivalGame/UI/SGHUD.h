// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SGHUD.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASGHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void PreInitializeComponents() override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
};
