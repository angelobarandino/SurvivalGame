﻿using UnrealBuildTool;

public class SurvivalGame : ModuleRules
{
	public SurvivalGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",	
			"InputCore",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"ModularGameplay",
			"ModularGameplayActors",
			"GameFeatures"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput"
		});
	}
}