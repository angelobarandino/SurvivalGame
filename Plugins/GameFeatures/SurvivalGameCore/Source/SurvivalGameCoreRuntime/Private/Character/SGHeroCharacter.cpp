// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGHeroCharacter.h"

#include "Character/SGHeroComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SurvivalGame/Player/SGPlayerState.h"

ASGHeroCharacter::ASGHeroCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	HeroComponent = CreateDefaultSubobject<USGHeroComponent>("HeroComponent");
}

void ASGHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetPlayerState<ASGPlayerState>()->InitAbilityActorInfo(this);
}

void ASGHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	GetPlayerState<ASGPlayerState>()->InitAbilityActorInfo(this);
}
