// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "InteractionStatics.h"


AInteractableActor::AInteractableActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ActiveInteractingActorCount(0)
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

void AInteractableActor::GetInteractionDefinition(FInteractionDefinition& InteractionDefinition)
{
	InteractionDefinition.Name = InteractableObjectName;
	InteractionDefinition.InteractableTarget = this;
	InteractionDefinition.InteractWidgetClass = InteractWidgetClass;

	for (const FInteractOption& InteractOption : InteractOptions)
	{
		FInteractOption& Option = InteractionDefinition.InteractOptions.Add_GetRef(InteractOption);
		UInteractionStatics::SetInteractOptionData(Option);
		Option.InteractableTarget = this;
	}
}

const FText AInteractableActor::GetInteractableName() const
{
	return FText::FromString(InteractableObjectName.ToString());
}
