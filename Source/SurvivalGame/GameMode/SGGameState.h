// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturesSubsystem.h"
#include "ModularGameState.h"
#include "SGGameState.generated.h"

class USGExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSGExperienceLoaded, const USGExperienceDefinition* /*Experience*/);

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASGGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	ASGGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool IsExperienceLoaded() const;
	void SetCurrentExperience(const FPrimaryAssetId& ExperienceId);
	void CallOrRegister_OnExperienceLoaded(FOnSGExperienceLoaded::FDelegate&& Delegate);
	const USGExperienceDefinition* GetCurrentExperienceChecked() const;

protected:
	FOnSGExperienceLoaded OnExperienceLoaded;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperienceDefinition);
	TObjectPtr<const USGExperienceDefinition> CurrentExperience = nullptr;
	
	UFUNCTION()
	void OnRep_CurrentExperienceDefinition();

	void StartGameplayExperienceLoad();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceLoadComplete();
	void DeactivateGameFeaturePlugins();
	
private:
	TArray<FString> GameFeaturePluginUrls;
	int32 NumGameFeaturePluginLoading = 0;

	void CollectGameFeaturePlugin(TArray<FString>& OutGameFeaturePluginUrls) const;
};
