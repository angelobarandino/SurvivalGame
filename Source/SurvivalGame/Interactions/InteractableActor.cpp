﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "InteractionStatics.h"


AInteractableActor::AInteractableActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableActor::HighlightActor()
{
	UInteractionStatics::SetActorHighlightEnable(this, true);
}

void AInteractableActor::UnHighlightActor()
{
	UInteractionStatics::SetActorHighlightEnable(this, false);
}

void AInteractableActor::GatherInteractionOptions(TArray<FInteractOption>& InteractionOptions)
{
	for (const FInteractOption& InteractOption : InteractOptions)
	{
		FInteractOption& OptionEntry = InteractionOptions.Add_GetRef(InteractOption);
		OptionEntry.InteractableTarget = this;
	}
}