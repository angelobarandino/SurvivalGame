// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeatureAction_AddLevelInstances.h"

#include "CoreTypes.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"
#include "HAL/PlatformCrt.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Text.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Misc/AssertionMacros.h"
#include "SurvivalGame/SurvivalGame.h"
#include "Trace/Detail/Channel.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectBaseUtility.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

class UGameFrameworkComponentManager;
struct FGameFeatureDeactivatingContext;

#define LOCTEXT_NAMESPACE "AncientGameFeatures"

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddLevelInstances

void UGameFeatureAction_AddLevelInstances::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ActorData.IsEmpty()) ||
		!ensure(ActiveData.ComponentRequests.IsEmpty()))
	{
		Reset(ActiveData);
	}

	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddLevelInstances::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddLevelInstances::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const FGameFeatureLevelInstanceEntry& Entry : LevelInstanceList)
	{
		if (Entry.Level.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("LevelEntryNull", "Null level reference at index {0} in LevelInstanceList"), FText::AsNumber(EntryIndex)));
		}

		++EntryIndex;
	}

	return Result;
}
#endif

void UGameFeatureAction_AddLevelInstances::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	const UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((GameInstance != nullptr) && World && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			const UGameFrameworkComponentManager::FExtensionHandlerDelegate ExtensionDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext);

			const TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
				ComponentManager->AddExtensionHandler(APlayerController::StaticClass(), ExtensionDelegate);

			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddLevelInstances::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	
	for (TTuple<FObjectKey, FPerActorData>& ActorData : ActiveData.ActorData)
	{
		for (ULevelStreamingDynamic* Level : ActorData.Value.AddedLevels)
		{
			CleanUpAddedLevel(Level);
		}
	}
	
	ActiveData.ActorData.Empty();
}

void UGameFeatureAction_AddLevelInstances::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveLevelInstance(Actor, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
	{
		AddLevelInstance(Actor, ActiveData);
	}
}

void UGameFeatureAction_AddLevelInstances::AddLevelInstance(const AActor* Actor, FPerContextData& ActiveData)
{
	UWorld* World = Actor->GetWorld();
	const UGameInstance* GameInstance = Actor->GetGameInstance();
	
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		FPerActorData& ActorData = ActiveData.ActorData.FindOrAdd(Actor);
		ActorData.AddedLevels.Reserve(LevelInstanceList.Num());
		
		for (const FGameFeatureLevelInstanceEntry& Entry : LevelInstanceList)
		{
			if (!Entry.Level.IsNull())
			{
				if (const UWorld* TargetWorld = Entry.TargetWorld.Get())
				{
					if (TargetWorld != World)
					{
						continue;
					}

					FString LevelName;
					if (!Entry.LevelName.IsEmpty())
					{
						LevelName = Entry.LevelName + TEXT("_StreamingLevel");;
					}
					
					bool bSuccess = false;
					ULevelStreamingDynamic* StreamingLevelRef = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(World, Entry.Level, Entry.Location, Entry.Rotation, bSuccess, LevelName);

					const FString ActorRoleName = GetActorRole(Actor);
					if (!bSuccess)
					{
						UE_LOG(LogSurvivalGame, Error, TEXT("[GameFeatureData %s]: Failed to load level instance `%s` on Actor `%s` [ROLE: %s]."), *GetPathNameSafe(this), *Entry.Level.ToString(), *GetNameSafe(Actor), *ActorRoleName);
					}

					if (bSuccess)
					{
						UE_LOG(LogSurvivalGame, Log, TEXT("[GameFeatureData %s]: Success loading level instance `%s` on Actor `%s` [ROLE: %s]."), *GetPathNameSafe(this), *Entry.Level.ToString(), *GetNameSafe(Actor), *ActorRoleName);
						ActorData.AddedLevels.Add(StreamingLevelRef);
					}
				}
			}
		}
	}
	
	GEngine->BlockTillLevelStreamingCompleted(World);
}

void UGameFeatureAction_AddLevelInstances::RemoveLevelInstance(const AActor* Actor, FPerContextData& ActiveData) const
{
	if (FPerActorData* ActorData = ActiveData.ActorData.Find(Actor))
	{
		for (ULevelStreamingDynamic* Level : ActorData->AddedLevels)
		{
			CleanUpAddedLevel(Level);
		}
		
		ActiveData.ActorData.Remove(Actor);
	}
}

void UGameFeatureAction_AddLevelInstances::CleanUpAddedLevel(ULevelStreamingDynamic* Level) const
{
	if (Level)
	{
		Level->OnLevelLoaded.RemoveAll(this);
		Level->SetIsRequestingUnloadAndRemoval(true);
	}
}

FString UGameFeatureAction_AddLevelInstances::GetActorRole(const AActor* Actor) const
{
	FString RoleString;
	switch (Actor->GetLocalRole()) {
	case ROLE_None:
		RoleString = TEXT("None");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		RoleString = TEXT("Simulated Only");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		RoleString = TEXT("Autonomous Proxy");
		break;
	case ENetRole::ROLE_Authority:
		RoleString = TEXT("Authority");
		break;
	case ENetRole::ROLE_MAX:
		RoleString = TEXT("Invalid Role");
		break;
	}
	return RoleString;
}

//////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE