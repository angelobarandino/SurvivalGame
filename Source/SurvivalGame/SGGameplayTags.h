﻿#pragma once
#include "NativeGameplayTags.h"

namespace SGGameplayTags
{
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Rotate);
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Zoom);

	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Active)
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Primary)
	SURVIVALGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Secondary)
	
}
