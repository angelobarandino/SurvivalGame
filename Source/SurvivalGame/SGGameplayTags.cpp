
#include "SGGameplayTags.h"

namespace SGGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Input Move");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Rotate, "InputTag.Rotate", "Camera Rotate");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Zoom, "InputTag.Zoom", "Camera Zoom");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Ability_Interact_Primary, "InputTag.Ability.Interact.Primary", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Ability_Interact_Secondary, "InputTag.Ability.Interact.Secondary", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Active, "Ability.Interaction.Active", "Requirement tag for activating interaction option");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Primary, "Ability.Interaction.Primary", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Secondary, "Ability.Interaction.Secondary", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_WaitForInput, "Ability.Interaction.WaitForInput", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Blocked, "Ability.Interaction.Blocked", "Use to disable interaction scan");
}
