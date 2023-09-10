// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEComputerVisionModule.h"
#include "Modules/ModuleManager.h"
#include <windows.h>

void UEComputerVisionModule::StatupUEComputerVisionModule()
{
	const FString LibComputerVisionModule = FPaths::Combine(*BasePluginDir, TEXT("Binaries/Win64/ComputerVisionModule.dll"));
	DynamicLibComputerVisionModuleHandle = FPlatformProcess::GetDllHandle(*LibComputerVisionModule);

	if (DynamicLibComputerVisionModuleHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("ComputerVisionModule.dll loaded successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("ComputerVisionModule.dll failed to load!"));
	}
}

void UEComputerVisionModule::ShutdownUEComputerVisionModule()
{
	FPlatformProcess::FreeDllHandle(DynamicLibComputerVisionModuleHandle);
	DynamicLibComputerVisionModuleHandle = nullptr;
}

void UEComputerVisionModule::StartupModule()
{
	StatupUEComputerVisionModule();
}

void UEComputerVisionModule::ShutdownModule()
{
	ShutdownUEComputerVisionModule();
}

bool UEComputerVisionModule::Initialize()
{
	nets.Add(FString("red"), BasePluginDir + FString("\\Models\\OpenCV\\red\\best.onnx"));
	nets.Add(FString("blue"), BasePluginDir + FString("\\Models\\OpenCV\\blue\\best.onnx"));

	ComputerVisionModuleDLL_Run = (float(*)(const std::string&, const std::string&, const int, const int))FPlatformProcess::GetDllExport(DynamicLibComputerVisionModuleHandle, TEXT("Run"));
	return (ComputerVisionModuleDLL_Run) ? true : false;
}

float UEComputerVisionModule::Run()
{
	return ComputerVisionModuleDLL_Run(TCHAR_TO_UTF8(*BasePluginDir), TCHAR_TO_UTF8(*primaryModelName), IMG_HEIGHT, IMG_WIDTH);
}

void UEComputerVisionModule::SetPrimaryModel(const FString& modelName)
{
    primaryModelName = modelName;
    UE_LOG(LogTemp, Display, TEXT("SetPrimaryModel"));
}

IMPLEMENT_MODULE(UEComputerVisionModule, UEComputerVision);
 