// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Assignment2EditorTarget : TargetRules
{
	public Assignment2EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.Add("Assignment2");
	}
}
