// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivalGame/Interactions/InteractableTarget.h"
#include "InteractableComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAME_API UInteractableComponent : public UActorComponent, public IInteractableTarget
{
	GENERATED_BODY()

public:
	UInteractableComponent(const FObjectInitializer& ObjectInitializer);
	
	// ~ Begin IInteractableTarget
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void GatherInteractionOptions(TArray<FInteractOption>& InteractionOptions) override;
	// ~ End IInteractableTarget

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<FInteractOption> InteractOptions;
};
