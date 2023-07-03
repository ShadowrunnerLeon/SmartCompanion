// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmartCompanionModule.h"
#include "Modules/ModuleManager.h"

void SmartCompanionModule::StartupVosk()
{
	const FString LibVoskPath = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/libvosk.dll"));
	DynamicLibVoskHandle = FPlatformProcess::GetDllHandle(*LibVoskPath);

	if (DynamicLibVoskHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("libvosk.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("libvosk.dll failed to load!"));
	}
}

void SmartCompanionModule::StatupPortAudio()
{
	const FString LibPortAudioPath = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/portaudio_x64.dll"));
	DynamicLibPortAudioHandle = FPlatformProcess::GetDllHandle(*LibPortAudioPath);

	if (DynamicLibPortAudioHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("portaudio_x64.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("portaudio_x64.dll failed to load!"));
	}
}

void SmartCompanionModule::StatupOpenCV()
{
	const FString LibOpenCVPath = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/opencv_world470.dll"));
	DynamicLibOpenCVHandle = FPlatformProcess::GetDllHandle(*LibOpenCVPath);

	if (DynamicLibOpenCVHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("opencv_world470.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("opencv_world470.dll failed to load!"));
	}
}

void SmartCompanionModule::ShutdownVosk()
{
	FPlatformProcess::FreeDllHandle(DynamicLibVoskHandle);
	DynamicLibVoskHandle = nullptr;
}

void SmartCompanionModule::ShutdownPortAudio()
{
	FPlatformProcess::FreeDllHandle(DynamicLibPortAudioHandle);
	DynamicLibPortAudioHandle = nullptr;
}

void SmartCompanionModule::ShutdownOpenCV()
{
	FPlatformProcess::FreeDllHandle(DynamicLibOpenCVHandle);
	DynamicLibOpenCVHandle = nullptr;
}

void SmartCompanionModule::StartupModule()
{
	StartupVosk();
	StatupPortAudio();
	StatupOpenCV();
}

void SmartCompanionModule::ShutdownModule()
{
	ShutdownVosk();
	ShutdownPortAudio();
	ShutdownOpenCV();
}

IMPLEMENT_PRIMARY_GAME_MODULE(SmartCompanionModule, SmartCompanion, "SmartCompanion" );
 