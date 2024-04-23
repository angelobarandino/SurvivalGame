// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurvivalGame/Interactions/InteractableActor.h"
#include "ItemPickup.generated.h"

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API AItemPickup : public AInteractableActor
{
	GENERATED_BODY()

public:
	AItemPickup(const FObjectInitializer& ObjectInitializer);

};
