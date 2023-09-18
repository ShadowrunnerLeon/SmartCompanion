// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class UESpeechRecognition : ModuleRules
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

    public void LoadSpeechRecognitionModule(ReadOnlyTargetRules Target)
    {
        PublicAdditionalLibraries.Add(baseDir + "\\ThirdParty\\SpeechRecognitionModule\\lib\\SpeechRecognitionModule.lib");
        RuntimeDependencies.Add(baseDir + "\\ThirdParty\\SpeechRecognitionModule\\bin\\SpeechRecognitionModule.dll");

        CopyToBinaries(baseDir + "\\ThirdParty\\SpeechRecognitionModule\\bin\\SpeechRecognitionModule.dll", Target);
        CopyToBinaries(baseDir + "\\ThirdParty\\PortAudio\\Lib\\Release\\portaudio_x64.dll", Target);
        CopyToBinaries(baseDir + "\\ThirdParty\\Vosk\\vosk-win64\\libgcc_s_seh-1.dll", Target);
        CopyToBinaries(baseDir + "\\ThirdParty\\Vosk\\vosk-win64\\libstdc++-6.dll", Target);
        CopyToBinaries(baseDir + "\\ThirdParty\\Vosk\\vosk-win64\\libwinpthread-1.dll", Target);
        CopyToBinaries(baseDir + "\\ThirdParty\\Vosk\\vosk-win64\\libvosk.dll", Target);
    }

    public UESpeechRecognition(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core"
        });

        LoadSpeechRecognitionModule(Target);
    }
}
