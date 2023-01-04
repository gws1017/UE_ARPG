// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_ARPG : ModuleRules
{
	public UE_ARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" ,"AIModule","AnimGraphRuntime" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.Add(ModuleDirectory);

	}
}
