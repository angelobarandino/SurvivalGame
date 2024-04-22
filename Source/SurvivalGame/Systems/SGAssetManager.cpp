// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "SurvivalGame/SurvivalGame.h"

USGAssetManager& USGAssetManager::Get()
{
	check(GEngine);
	if (USGAssetManager* Singleton = Cast<USGAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	
	UE_LOG(LogSurvivalGame, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini. It must be set to SGAssetManager!"));

	return *NewObject<USGAssetManager>();
}

void USGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
