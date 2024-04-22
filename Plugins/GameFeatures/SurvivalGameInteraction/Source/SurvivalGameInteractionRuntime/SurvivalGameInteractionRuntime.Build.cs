// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalGameInteractionRuntime : ModuleRules
{
	public SurvivalGameInteractionRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {});
				
		PrivateIncludePaths.AddRange(new string[] {});
			
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"SurvivalGame",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore"
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[] {});
	}
}
