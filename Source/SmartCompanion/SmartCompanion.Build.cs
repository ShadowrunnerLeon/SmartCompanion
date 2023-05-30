// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class SmartCompanion : ModuleRules
{
    private void CopyToBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string binariesDir = Path.Combine("D:\\SmartCompanion\\SmartCompanion", "Binaries", Target.Platform.ToString());
        string filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(binariesDir))
            Directory.CreateDirectory(binariesDir);

        if (!File.Exists(Path.Combine(binariesDir, filename)))
            File.Copy(Filepath, Path.Combine(binariesDir, filename), true);
    }

    public void LoadVosk(ReadOnlyTargetRules Target)
    {
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libvosk.lib");
        PublicIncludePaths.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64");
        RuntimeDependencies.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libvosk.dll");
        PublicDelayLoadDLLs.Add("libvosk.dll");

        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libgcc_s_seh-1.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libstdc++-6.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libwinpthread-1.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libvosk.dll", Target);
    }

    public void LoadPortAudio(ReadOnlyTargetRules Target)
    {
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\PortAudio\\Lib\\Release\\portaudio_static_x64.lib");
        PublicIncludePaths.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\PortAudio\\Include");
        RuntimeDependencies.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\PortAudio\\Lib\\Release\\portaudio_x64.dll");
        PublicDelayLoadDLLs.Add("portaudio_x64.dll");

        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\PortAudio\\Lib\\Release\\portaudio_x64.dll", Target);
    }

    public void LoadOpenCV(ReadOnlyTargetRules Target)
    {
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_world470.lib");
        PublicIncludePaths.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\include");
        RuntimeDependencies.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_world470.dll");
        PublicDelayLoadDLLs.Add("opencv_world470.dll");

        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_world470.dll", Target);
    }

    public SmartCompanion(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay"
        });

        LoadVosk(Target);
        LoadPortAudio(Target);
        LoadOpenCV(Target);
    }
}
