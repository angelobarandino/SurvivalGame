// Fill out your copyright notice in the Description page of Project Settings.


#include "SGPlayerController.h"

#include "SGPlayerState.h"

ASGPlayerController::ASGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	bShowMouseCursor = true;
}

UAbilitySystemComponent* ASGPlayerController::GetAbilitySystemComponent() const
{
	if (const ASGPlayerState* PS = GetPlayerState<ASGPlayerState>())
	{
		return PS->GetAbilitySystemComponent();
	}

	return nullptr;
}
