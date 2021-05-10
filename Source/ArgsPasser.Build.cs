// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ArgsPasser : ModuleRules
{
	public ArgsPasser(ReadOnlyTargetRules Target) : base(Target)
	{
		bEnforceIWYU = true;
		bLegacyPublicIncludePaths = false;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(Path.Combine(EngineDirectory, "Source", "Runtime/Launch/Public"));
		PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source", "Runtime/Launch/Private"));		// For LaunchEngineLoop.cpp include
		
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"AppFramework",
				"Core",
				"ApplicationCore",
				"Projects",
				"Slate",
				"SlateCore",
				"StandaloneRenderer",
				"DesktopPlatform",
				"SlateFileDialogs"
			}
		);

		if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.TVOS)
		{
			PrivateDependencyModuleNames.AddRange(
                new string [] {
                    "NetworkFile",
                    "StreamingFile"
                }
            );
		}

		if (Target.IsInPlatformGroup(UnrealPlatformGroup.Linux))
		{
			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"UnixCommonStartup"
				}
			);
		}
	}
}
