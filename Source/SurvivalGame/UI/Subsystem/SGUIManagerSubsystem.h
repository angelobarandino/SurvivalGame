// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "SGUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API USGUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:

	USGUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	FTSTicker::FDelegateHandle TickHandle;
	
	bool Tick(float DeltaTime);
};
