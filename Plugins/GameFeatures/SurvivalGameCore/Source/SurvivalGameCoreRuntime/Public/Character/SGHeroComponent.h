// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "SGHeroComponent.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class USGInputConfig;
/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SURVIVALGAMECORERUNTIME_API USGHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	USGHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnRegister() override;


	UPROPERTY(EditDefaultsOnly, Category = "Hero|Input")
	TObjectPtr<USGInputConfig> DefaultInputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Hero|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Hero")
	float ZoomSpeed = 3.f;

	UPROPERTY(EditDefaultsOnly, Category="Hero")
	float ZoomStepSize = 300.f;

	UPROPERTY(EditDefaultsOnly, Category="Hero")
	float ZoomMin = 400;
	
	UPROPERTY(EditDefaultsOnly, Category="Hero")
	float ZoomMax = 1600;
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** The name of this overall feature */
	static const FName NAME_ActorFeatureName;
	
	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual void CheckDefaultInitialization() override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	//~ End IGameFrameworkInitStateInterface interface
	
	void InitializeDefaultPlayerInput(UInputComponent* PlayerInputComponent);

private:
	FTimerHandle ZoomTimerHandle;
	float TargetZoomDistance = 800.f;
	bool bDefaultPlayerInputInitialized = false;
	TArray<uint32> AbilityInputBindHandles;
	
	void BindAdditionalInputConfig(const USGInputConfig* InputConfig);
	void RemoveInputBindings();
	
	void Input_Move(const FInputActionValue& Value);
	void Input_Rotate(const FInputActionValue& Value);
	void Input_Zoom(const FInputActionValue& Value);
	void OnZoom();
	
	void Input_AbilityPressed(FGameplayTag InputTag);
	void Input_AbilityReleased(FGameplayTag InputTag);
};
