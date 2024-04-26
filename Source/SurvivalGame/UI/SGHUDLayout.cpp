// Fill out your copyright notice in the Description page of Project Settings.


#include "SGHUDLayout.h"

#include "CommonUIExtensions.h"
#include "NativeGameplayTags.h"
#include "Input/CommonUIInputTypes.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_Layer_Menu, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_Action_Escape, "UI.Action.Escape");

USGHUDLayout::USGHUDLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USGHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//const FBindUIActionArgs EscapeAction(FUIActionTag::ConvertChecked(TAG_UI_Action_Escape), false, FSimpleDelegate::CreateUObject(this, &USGHUDLayout::HandleEscapeAction));
	//RegisterUIActionBinding(EscapeAction);
}

void USGHUDLayout::HandleEscapeAction() const
{
	if (ensure(!EscapeMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_Layer_Menu, EscapeMenuClass);
	}
}
