// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "InteractableScannerComponent.generated.h"


class IInteractableTarget;

DECLARE_MULTICAST_DELEGATE_OneParam(FInteractableTargetDelegate, const TScriptInterface<IInteractableTarget>);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAME_API UInteractableScannerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UInteractableScannerComponent(const FObjectInitializer& ObjectInitializer);

	FInteractableTargetDelegate FoundInteractable;
	FInteractableTargetDelegate LostInteractable;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionScanRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionScanRate = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bShowDebug = false;

	TScriptInterface<IInteractableTarget> GetActiveInteractableTarget() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	FTimerHandle TimerHandle;
	TScriptInterface<IInteractableTarget> ActiveInteractableTarget;
	
	void ToggleInteractableHighlight(const TScriptInterface<IInteractableTarget>& InteractableObject, const bool bIsHighligh) const;
	
	void PerformTrace();
	void FoundInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableObject);
	void LostInteractableTarget();
};
