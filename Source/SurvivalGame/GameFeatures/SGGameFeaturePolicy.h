#pragma once

#include "GameFeaturesProjectPolicies.h"
#include "GameFeatureStateChangeObserver.h"
#include "SGGameFeaturePolicy.generated.h"

UCLASS(MinimalAPI, Config = Game)
class USGGameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
	
public:
	USGGameFeaturePolicy(const FObjectInitializer& ObjectInitializer);
	
	SURVIVALGAME_API static USGGameFeaturePolicy& Get();

	//~UGameFeaturesProjectPolicies interface
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;
	virtual void GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const override;
	//~End of UGameFeaturesProjectPolicies interface

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

UCLASS()
class UGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
};
