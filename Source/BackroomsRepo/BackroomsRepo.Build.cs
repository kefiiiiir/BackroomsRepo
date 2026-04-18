// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BackroomsRepo : ModuleRules
{
	public BackroomsRepo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "PhysicsCore" });
	}
}
