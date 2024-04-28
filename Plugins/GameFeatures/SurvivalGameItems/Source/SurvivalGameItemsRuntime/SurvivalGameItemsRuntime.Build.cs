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
			"SurvivalGame",
			"GameplayMessageRuntime"
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
			"GameplayTags",
			"CommonGame",
			"CommonUI"
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
			
		});
	}
}
