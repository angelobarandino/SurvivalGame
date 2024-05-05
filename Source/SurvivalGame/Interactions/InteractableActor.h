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
	virtual void GetInteractionDefinition(FInteractionDefinition& InteractionDefinition) override;
	virtual const FText GetInteractableName() const override;
	// ~End IInteractableTarget
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionActive();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionInactive();
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName InteractableObjectName;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSoftClassPtr<UUserWidget> InteractWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<FInteractOption> InteractOptions;

private:
	int32 ActiveInteractingActorCount;
};
