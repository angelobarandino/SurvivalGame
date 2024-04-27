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
			"Slate",
			"SlateCore",
			"UMG"
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
			
		});
	}
}
