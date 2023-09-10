// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class UEComputerVision : ModuleRules
{
    private const string baseDir = "E:\\SmartCompanion";

    private void CopyToBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string binariesDir = Path.Combine(baseDir, "Binaries", Target.Platform.ToString());
        string filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(binariesDir))
            Directory.CreateDirectory(binariesDir);

        if (!File.Exists(Path.Combine(binariesDir, filename)))
            File.Copy(Filepath, Path.Combine(binariesDir, filename), true);
    }

    public void LoadComputerVisionModule(ReadOnlyTargetRules Target)
    {
        PublicAdditionalLibraries.Add(baseDir + "\\ThirdParty\\ComputerVisionModule\\lib\\ComputerVisionModule.lib");
        RuntimeDependencies.Add(baseDir + "\\ThirdParty\\ComputerVisionModule\\bin\\ComputerVisionModule.dll");
        PublicDelayLoadDLLs.Add("ComputerVisionModule.dll");

        PublicIncludePaths.Add(baseDir + "\\ThirdParty\\OpenCV\\include");

        CopyToBinaries(baseDir + "\\ThirdParty\\ComputerVisionModule\\bin\\ComputerVisionModule.dll", Target);
    }

    public UEComputerVision(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core"
        });

        LoadComputerVisionModule(Target);
    }
}
