// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StaticMeshSequencePlay : ModuleRules
{
	public StaticMeshSequencePlay(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = false;
		
		PrivateIncludePaths.AddRange(
		    new string[] {
				"StaticMeshSequencePlay/Private"
			});

		PrivateDependencyModuleNames.AddRange(
		    new string[] {
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore"
			});
	}
}