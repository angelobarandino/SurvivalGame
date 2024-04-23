// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableTarget.h"
#include "InteractOption.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS(Abstract)
class SURVIVALGAME_API AInteractableActor : public AActor, public IInteractableTarget
{
	GENERATED_BODY()

public:
	AInteractableActor(const FObjectInitializer& ObjectInitializer);

	// ~Begin IInteractableTarget
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void GatherInteractionOptions(TArray<FInteractOption>& InteractionOptions) override;
	// ~End IInteractableTarget

protected:
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<FInteractOption> InteractOptions;

};
