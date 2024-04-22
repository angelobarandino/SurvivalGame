// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameInstance.h"

#include "Components/GameFrameworkComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "SurvivalGame/SGGameplayTags.h"

USGGameInstance::USGGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USGGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(SGGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(SGGameplayTags::InitState_DataAvailable, false, SGGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(SGGameplayTags::InitState_DataInitialized, false, SGGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(SGGameplayTags::InitState_GameplayReady, false, SGGameplayTags::InitState_DataInitialized);
	}
}

void USGGameInstance::Shutdown()
{
	Super::Shutdown();
}
