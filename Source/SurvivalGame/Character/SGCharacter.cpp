// Fill out your copyright notice in the Description page of Project Settings.


#include "SGCharacter.h"

#include "SGPawnManagerComponent.h"
#include "SurvivalGame/AbilitySystem/SGAbilitySystemComponent.h"

ASGCharacter::ASGCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PawnManagerComponent = CreateDefaultSubobject<USGPawnManagerComponent>("PawnManager");
}

UAbilitySystemComponent* ASGCharacter::GetAbilitySystemComponent() const
{
	if (PawnManagerComponent)
	{
		return PawnManagerComponent->GetAbilitySystemComponent();
	}
	
	return nullptr;
}

void ASGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PawnManagerComponent->UpdateInitializationState();
}

void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnManagerComponent->UpdateInitializationState();
}

void ASGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	PawnManagerComponent->UpdateInitializationState();
}
