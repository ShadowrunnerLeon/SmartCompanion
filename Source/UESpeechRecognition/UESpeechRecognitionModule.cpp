// Copyright Epic Games, Inc. All Rights Reserved.

#include "UESpeechRecognitionModule.h"
#include "Modules/ModuleManager.h"

void UESpeechRecognitionModule::StartupUESpeechRecognitionModule()
{
	const FString LibSpeechRecognitionModule = FPaths::Combine("E:/SmartCompanion/", TEXT("Binaries/Win64/SpeechRecognitionModule.dll"));
	DynamicLibSpeechRecognitionModuleHandle = FPlatformProcess::GetDllHandle(*LibSpeechRecognitionModule);

	if (DynamicLibSpeechRecognitionModuleHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("SpeechRecognitionModule.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("SpeechRecognitionModule.dll failed to load!"));
	}
}

void UESpeechRecognitionModule::ShutdownUESpeechRecognitionModule()
{
	FPlatformProcess::FreeDllHandle(DynamicLibSpeechRecognitionModuleHandle);
	DynamicLibSpeechRecognitionModuleHandle = nullptr;
}

void UESpeechRecognitionModule::StartupModule()
{
	StartupUESpeechRecognitionModule();

	Initialize = (const char*(*)())(FPlatformProcess::GetDllExport(DynamicLibSpeechRecognitionModuleHandle, TEXT("Initialize")));
	Run = (const char* (*)())(FPlatformProcess::GetDllExport(DynamicLibSpeechRecognitionModuleHandle, TEXT("Run")));
	Shutdown = (void(*)())(FPlatformProcess::GetDllExport(DynamicLibSpeechRecognitionModuleHandle, TEXT("Shutdown")));

	FString resInitialize(Initialize());
	UE_LOG(LogTemp, Display, TEXT("%s"), FString(resInitialize));
}

void UESpeechRecognitionModule::ShutdownModule()
{
	Shutdown();
	ShutdownUESpeechRecognitionModule();
}

IMPLEMENT_MODULE(UESpeechRecognitionModule, UESpeechRecognition);
 