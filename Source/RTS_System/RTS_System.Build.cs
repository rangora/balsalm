// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RTS_System : ModuleRules
{
	public RTS_System(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", 
			"AIModule", "NavigationSystem", "Astar", "UMG", "Slate", "SlateCore", "RenderCore", "RHI", "FogOFWar" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //string HeaderPaths[] = new string[] { "ActorType.h" };
        //string HeaderPath = "ActorType.h";

        ////PrivateIncludePaths.Add(HeaderPath);
        //PublicIncludePaths.Add(HeaderPath);

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
