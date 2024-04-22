// Fill out your copyright notice in the Description page of Project Settings.


#include "SGPlayerController.h"

ASGPlayerController::ASGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	bShowMouseCursor = true;
}
