// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPromptWidget.h"

#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UInteractionPromptWidget::PushContentToInteractionPrompt(TSubclassOf<UCommonActivatableWidget> WidgetClass, const UObject* ContentData)
{
	UCommonActivatableWidget* ContentWidget = InteractionWidgetStack->AddWidget(WidgetClass);
	if (ContentWidget->Implements<UInteractionWidgetInterface>())
	{
		IInteractionWidgetInterface::Execute_OnSetContentData(ContentWidget, ContentData);
	}
}
