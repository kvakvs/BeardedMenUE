// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class BeardedMen : ModuleRules
{
	public BeardedMen(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent"
        });

	PrivateDependencyModuleNames.AddRange(new string[] {  });

        var base_path = Path.GetFullPath(ModuleDirectory);
        PublicIncludePaths.Add(Path.Combine(base_path, "PolyVox/include"));
        PublicIncludePaths.Add(Path.Combine(base_path, "Spatial"));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }
    }
}
