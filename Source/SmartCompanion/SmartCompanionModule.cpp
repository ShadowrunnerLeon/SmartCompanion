// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmartCompanionModule.h"
#include "Modules/ModuleManager.h"

void SmartCompanionModule::StartupModule()
{
	const FString BasePluginDir = "D:/SmartCompanion/SmartCompanion";

	const FString LibVoskPath = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/libvosk.dll"));
	const FString LibPortAudioPath = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/portaudio_x64.dll"));
	//const FString LibOpenCVPath = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/opencv_world470.dll"));

	DynamicLibVoskHandle = FPlatformProcess::GetDllHandle(*LibVoskPath);
	DynamicLibPortAudioHandle = FPlatformProcess::GetDllHandle(*LibPortAudioPath);
	//DynamicLibOpenCVHandle = FPlatformProcess::GetDllHandle(*LibOpenCVPath);

	if (DynamicLibVoskHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("libvosk.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("libvosk.dll failed to load!"));
	}

	if (DynamicLibPortAudioHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("portaudio_x64.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("portaudio_x64.dll failed to load!"));
	}
	/*
	if (DynamicLibOpenCVHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("opencv_world470.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("opencv_world470.dll failed to load!"));
	}
	*/
}

void SmartCompanionModule::ShutdownModule()
{
	FPlatformProcess::FreeDllHandle(DynamicLibVoskHandle);
	DynamicLibVoskHandle = nullptr;

	FPlatformProcess::FreeDllHandle(DynamicLibPortAudioHandle);
	DynamicLibPortAudioHandle = nullptr;
	/*
	FPlatformProcess::FreeDllHandle(DynamicLibOpenCVHandle);
	DynamicLibOpenCVHandle = nullptr;
	*/
}

IMPLEMENT_PRIMARY_GAME_MODULE(SmartCompanionModule, SmartCompanion, "SmartCompanion" );
 