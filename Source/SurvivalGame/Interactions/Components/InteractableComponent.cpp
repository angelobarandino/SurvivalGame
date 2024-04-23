﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "SurvivalGame/Interactions/InteractionStatics.h"

UInteractableComponent::UInteractableComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInteractableComponent::HighlightActor()
{
	UInteractionStatics::SetActorHighlightEnable(GetOwner(), true);
}

void UInteractableComponent::UnHighlightActor()
{
	UInteractionStatics::SetActorHighlightEnable(GetOwner(), false);
}

void UInteractableComponent::GatherInteractionOptions(TArray<FInteractOption>& InteractionOptions)
{
	for (const FInteractOption& InteractOption : InteractOptions)
    {
    	FInteractOption& OptionEntry = InteractionOptions.Add_GetRef(InteractOption);
    	OptionEntry.InteractableTarget = this;
    }
}
