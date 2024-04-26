// Fill out your copyright notice in the Description page of Project Settings.


#include "SGActivatableWidget.h"

USGActivatableWidget::USGActivatableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

TOptional<FUIInputConfig> USGActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputMode)
	{
	case EWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
