// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "SurvivalGame/Interactions/InteractionStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractableComponent)

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
    }
}

void UInteractableComponent::GetInteractionDefinition(FInteractionDefinition& InteractionDefinition)
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

const FText UInteractableComponent::GetInteractableName() const
{
	return FText::FromString(InteractableObjectName.ToString());
}

