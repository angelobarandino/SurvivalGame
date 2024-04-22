// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableTarget.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAMEINTERACTIONRUNTIME_API UInteractableComponent : public UActorComponent, public IInteractableTarget
{
	GENERATED_BODY()

public:
	UInteractableComponent(const FObjectInitializer& ObjectInitializer);
	
	// ~ Begin IInteractableTarget
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// ~ End IInteractableTarget

protected:
	void SetOwnerStaticMeshRenderCustomDepth(bool bValue) const;
};
