// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalGameItemsRuntime : ModuleRules
{
	public SurvivalGameItemsRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[]
		{
			
		});
		
		PrivateIncludePaths.AddRange(new string[]
		{
			
		});
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"SurvivalGame"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Slate",
			"SlateCore",
			"UMG",
			"GameplayMessageRuntime",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"CommonGame",
			"CommonUI"
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
			
		});
	}
}
