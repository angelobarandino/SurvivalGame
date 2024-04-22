// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameState.h"

#include "GameFeaturesSubsystem.h"
#include "SGExperienceDefinition.h"
#include "Net/UnrealNetwork.h"
#include "SurvivalGame/Systems/SGAssetManager.h"

ASGGameState::ASGGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ASGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentExperience);
}

void ASGGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeactivateGameFeaturePlugins();
}

bool ASGGameState::IsExperienceLoaded() const
{
	return CurrentExperience != nullptr;
}

void ASGGameState::SetCurrentExperience(const FPrimaryAssetId& ExperienceId)
{
	const USGAssetManager& AssetManager = USGAssetManager::Get();
	const FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	const TSubclassOf<USGExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);

	const USGExperienceDefinition* ExperienceDefinition = GetDefault<USGExperienceDefinition>(AssetClass);
	check(ExperienceDefinition);
	check(CurrentExperience == nullptr);
	CurrentExperience = ExperienceDefinition;
	StartGameplayExperienceLoad();
}

void ASGGameState::CallOrRegister_OnExperienceLoaded(FOnSGExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

const USGExperienceDefinition* ASGGameState::GetCurrentExperienceChecked() const
{
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}

void ASGGameState::OnRep_CurrentExperienceDefinition()
{
	StartGameplayExperienceLoad();
}

void ASGGameState::StartGameplayExperienceLoad()
{
	check(CurrentExperience);

	GameFeaturePluginUrls.Empty();
	CollectGameFeaturePlugin(GameFeaturePluginUrls);
	
	NumGameFeaturePluginLoading = GameFeaturePluginUrls.Num();
	if (NumGameFeaturePluginLoading > 0)
	{
		for (const FString& PluginUrl : GameFeaturePluginUrls)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginUrl, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));			
		}
	}
	else
	{
		OnExperienceLoadComplete();
	}
}

void ASGGameState::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginLoading--;
	if (NumGameFeaturePluginLoading == 0)
	{
		OnExperienceLoadComplete();
	}
}

void ASGGameState::OnExperienceLoadComplete()
{
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

void ASGGameState::DeactivateGameFeaturePlugins()
{
	for (const FString& PluginUrl : GameFeaturePluginUrls)
	{
		UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginUrl);
	}
}

void ASGGameState::CollectGameFeaturePlugin(TArray<FString>& OutGameFeaturePluginUrls) const
{
	check(CurrentExperience)
	
	for (const FString& PluginName : CurrentExperience->GameFeaturesToEnable)
	{
		FString PluginURL;
		if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
		{
			OutGameFeaturePluginUrls.AddUnique(PluginURL);
		}
		else
		{
			ensureMsgf(false, TEXT("Failed to find plugin URL from PluginName %s for experience %s - fix data, ignoring for this run"), *PluginName, *CurrentExperience->GetPrimaryAssetId().ToString());
		}
	}
}

