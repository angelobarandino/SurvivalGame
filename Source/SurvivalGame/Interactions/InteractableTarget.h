﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractOption.h"
#include "UObject/Interface.h"
#include "InteractableTarget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVALGAME_API IInteractableTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void UnHighlightActor() = 0;
	virtual void HighlightActor() = 0;
	virtual void GatherInteractionOptions(TArray<FInteractOption>& InteractionOptions) = 0;
};