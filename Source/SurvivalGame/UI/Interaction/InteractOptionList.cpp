// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractOptionList.h"

#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"
#include "SurvivalGame/Interactions/InteractOption.h"

void UInteractOptionList::BuildInteractOptions(const TArray<FInteractOption>& InteractOptions)
{
	ClearChildren();
	
	for (const FInteractOption& Option : InteractOptions)
	{
		AddChild(CreateOption(Option));
	}
}

UWidget* UInteractOptionList::CreateOption(const FInteractOption& Option)
{
	UHorizontalBox* HBox = NewObject<UHorizontalBox>(this);

	UCommonTextBlock* InteractText = NewObject<UCommonTextBlock>();
	FSlateFontInfo Font;
	Font.Size = 14;
	InteractText->SetFont(Font);
	InteractText->SetText(Option.InteractText);
	HBox->AddChild(InteractText);
	
	return HBox;
}
