// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SmartCompanionTarget : TargetRules
{
	public SmartCompanionTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("SmartCompanion");
	}
}
