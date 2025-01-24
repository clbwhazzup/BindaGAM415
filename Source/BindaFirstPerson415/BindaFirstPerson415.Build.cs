// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BindaFirstPerson415 : ModuleRules
{
	public BindaFirstPerson415(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Niagara" });
	}
}
