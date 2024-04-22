// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalGameCoreRuntime : ModuleRules
{
	public SurvivalGameCoreRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] 
		{
			// ... add public include paths required here ...
		});
		
		PrivateIncludePaths.AddRange(new string[] 
		{
			// ... add other private include paths required here ...
		});
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"SurvivalGame",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"ModularGameplay",
			"ModularGameplayActors",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore", 
			"EnhancedInput",
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
			// ... add any modules that your module loads dynamically here ...
		});
	}
}
