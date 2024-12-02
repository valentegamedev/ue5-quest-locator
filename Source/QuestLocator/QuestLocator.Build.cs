// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuestLocator : ModuleRules
{
	public QuestLocator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "Niagara" });
	}
}
