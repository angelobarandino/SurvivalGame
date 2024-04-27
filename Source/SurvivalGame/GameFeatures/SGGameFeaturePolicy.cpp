#include "SGGameFeaturePolicy.h"

#include "AbilitySystemGlobals.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"
#include "SurvivalGame/AbilitySystem/SGGameplayCueManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SGGameFeaturePolicy)

USGGameFeaturePolicy::USGGameFeaturePolicy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

USGGameFeaturePolicy& USGGameFeaturePolicy::Get()
{
	return  UGameFeaturesSubsystem::Get().GetPolicy<USGGameFeaturePolicy>();
}

void USGGameFeaturePolicy::InitGameFeatureManager()
{
	Observers.Add(NewObject<UGameFeature_AddGameplayCuePaths>());
	
	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.AddObserver(Observer);
	}
	
	Super::InitGameFeatureManager();
}

void USGGameFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();
	
	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

void USGGameFeaturePolicy::GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const
{
	// Editor will load both, this can cause hitching as the bundles are set to not preload in editor
	bLoadClientData = !IsRunningDedicatedServer();
	bLoadServerData = !IsRunningClientOnly();
}



//////////////////////////////////////////////////////////////////////
//
void UGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCue = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
		{
			if (USGGameplayCueManager* GCM = USGGameplayCueManager::Get())
			{
				const UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
				const int32 PreInitializeNumCues = RuntimeGameplayCueSet
					? RuntimeGameplayCueSet->GameplayCueData.Num()
					: 0;
				
				const TArray<FDirectoryPath>& DirectoryPaths = AddGameplayCue->GetDirectoryPathsToAdd();
				for (const FDirectoryPath& DirectoryPath : DirectoryPaths)
				{
					FString MutablePath = DirectoryPath.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					GCM->AddGameplayCueNotifyPath(MutablePath, false);	
				}
				
				// Rebuild the runtime library with these new paths
				if (!DirectoryPaths.IsEmpty())
				{
					GCM->InitializeRuntimeObjectLibrary();	
				}
				
				const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
				if (PreInitializeNumCues != PostInitializeNumCues)
				{
					GCM->RefreshGameplayCuePrimaryAsset();
				}
			}
		}
	}
}

void UGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCue = Cast<UGameFeatureAction_AddGameplayCuePath>(GameFeatureData))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCue->GetDirectoryPathsToAdd();
			
			if (UGameplayCueManager* GCM = UAbilitySystemGlobals::Get().GetGameplayCueManager())
			{
				int32 NumRemoved = 0;
				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, false);
				}

				ensure(NumRemoved == DirsToAdd.Num());
				
				// Rebuild the runtime library only if there is a need to
				if (NumRemoved > 0)
				{
					GCM->InitializeRuntimeObjectLibrary();	
				}			
			}
		}
	}
}
