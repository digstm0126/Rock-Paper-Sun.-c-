// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Assignment2 : ModuleRules
{
	public Assignment2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new []{ "Assignment2" });
		PrivateIncludePaths.AddRange(new []{ "Assignment2" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
