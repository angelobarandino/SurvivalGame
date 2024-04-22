// Fill out your copyright notice in the Description page of Project Settings.


#include "SGWorldSettings.h"

#include "Engine/AssetManager.h"
#include "SurvivalGame/SurvivalGame.h"

ASGWorldSettings::ASGWorldSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

FPrimaryAssetId ASGWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId AssetId;
	if (!DefaultGameplayExperience.IsNull())
	{
		AssetId = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

		if (!AssetId.IsValid())
		{
			UE_LOG(LogSurvivalGame, Error,
				TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString()
			);
		}
	}

	return AssetId;
}
