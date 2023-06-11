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
        PublicIncludePaths.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\include");

        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_world470.lib");
        RuntimeDependencies.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_world470.dll");
        PublicDelayLoadDLLs.Add("opencv_world470.dll");

        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_world470.dll", Target);

        /*
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_core470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_highgui470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_imgcodecs470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_calib3d470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_imgproc470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_videoio470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_features2d470.lib");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_flann470.lib");

        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_core470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_highgui470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_imgcodecs470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_calib3d470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_imgproc470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_videoio470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_features2d470.dll");
        PublicAdditionalLibraries.Add("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_flann470.dll");


        PublicDelayLoadDLLs.Add("opencv_core470.dll");
        PublicDelayLoadDLLs.Add("opencv_highgui470.dll");
        PublicDelayLoadDLLs.Add("opencv_imgcodecs470.dll");
        PublicDelayLoadDLLs.Add("opencv_calib3d470.dll");
        PublicDelayLoadDLLs.Add("opencv_imgproc470.dll");
        PublicDelayLoadDLLs.Add("opencv_videoio470.dll");
        PublicDelayLoadDLLs.Add("opencv_features2d470.dll");
        PublicDelayLoadDLLs.Add("opencv_flann470.dll");

        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_core470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_highgui470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_imgcodecs470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_calib3d470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_imgproc470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_videoio470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_features2d470.dll", Target);
        CopyToBinaries("D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\bin\\opencv_flann470.dll", Target);
        */
    }

    public SmartCompanion(ReadOnlyTargetRules Target) : base(Target)
	{
        bEnableExceptions = true;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "RHI",
            "RenderCore"
        });

        LoadVosk(Target);
        LoadPortAudio(Target);
        LoadOpenCV(Target);
    }
}
