// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameMode.h"

#include "SGGameState.h"
#include "SGWorldSettings.h"
#include "SurvivalGame/SurvivalGame.h"
#include "SurvivalGame/Character/SGCharacter.h"
#include "SurvivalGame/Character/SGCharacterConfig.h"
#include "SurvivalGame/Character/SGPawnManagerComponent.h"
#include "SurvivalGame/Player/SGPlayerController.h"
#include "SurvivalGame/UI/SGHUD.h"

ASGGameMode::ASGGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ASGGameState::StaticClass();
	PlayerControllerClass = ASGPlayerController::StaticClass();
	PlayerStateClass = ASGPlayerController::StaticClass();
	DefaultPawnClass = ASGCharacter::StaticClass();
	HUDClass = ASGHUD::StaticClass();
}

void ASGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::LoadGameExperience);
}

UClass* ASGGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const USGCharacterConfig* CharacterConfig = GetCharacterConfigForController(InController))
	{
		return CharacterConfig->CharacterClass;
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ASGGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.ObjectFlags |= RF_Transient;
		SpawnParams.bDeferConstruction = true;

		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnParams))
		{
			if (USGPawnManagerComponent* PawnManager = SpawnedPawn->FindComponentByClass<USGPawnManagerComponent>())
			{
				if (const USGCharacterConfig* CharacterConfig = GetCharacterConfigForController(NewPlayer))
				{
					PawnManager->SetCharacterData(CharacterConfig);
				}
				else
				{
					UE_LOG(LogSurvivalGame, Error, TEXT("Game mode was unable to set CharacterData on the spawn pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}
			
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}
	else
	{
		UE_LOG(LogSurvivalGame, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}
	
	return nullptr;
}

void ASGGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

void ASGGameMode::InitGameState()
{
	Super::InitGameState();

	if (ASGGameState* GS = GetGameState<ASGGameState>())
	{
		GS->CallOrRegister_OnExperienceLoaded(FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnGameplayExperienceLoaded));
	}
}

const USGCharacterConfig* ASGGameMode::GetCharacterConfigForController(const AController* InController) const
{
	check(GameState);
	const ASGGameState* GS = GetGameState<ASGGameState>();
	if (GS->IsExperienceLoaded())
	{
		const USGExperienceDefinition* ExperienceDefinition = GS->GetCurrentExperienceChecked();
		return ExperienceDefinition->CharacterConfig;
	}

	return nullptr;
}

bool ASGGameMode::IsExperienceLoaded() const
{
	check(GameState);
	return GetGameState<ASGGameState>()->IsExperienceLoaded();
}

void ASGGameMode::LoadGameExperience() const
{
	if (const ASGWorldSettings* TypedWordSettings = Cast<ASGWorldSettings>(GetWorldSettings()))
	{
		FPrimaryAssetId ExperienceId = TypedWordSettings->GetDefaultGameplayExperience();
		if (ExperienceId.IsValid())
		{
			if (ASGGameState* GS = GetGameState<ASGGameState>())
			{
				GS->SetCurrentExperience(ExperienceId);
			}
		}
	}
}

void ASGGameMode::OnGameplayExperienceLoaded(const USGExperienceDefinition* ExperienceDefinition)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APlayerController* PC = Cast<APlayerController>(*Iterator))
		{
			if (PC->GetPawn() == nullptr)
			{
				if (PlayerCanRestart(PC))
				{
					RestartPlayer(PC);
				}
			}
		}
	}
}
