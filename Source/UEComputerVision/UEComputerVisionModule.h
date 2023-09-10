// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../ConfigData.h"
#include "../ISmartModule.h"

#define COMPUTERVISION_SINGLETON		((UEComputerVisionModule&)(FModuleManager::Get().LoadModuleChecked(TEXT("UEComputerVision"))))

class UEComputerVisionModule : public ISmartModule
{
	private:
		TMap<FString, FString> nets;
		FString primaryModelName;
		
		UWorld* worldContext;
		
		const FString BasePluginDir = "E:/SmartCompanion";
		void* DynamicLibComputerVisionModuleHandle;

		float (*ComputerVisionModuleDLL_Run)(const std::string&, const std::string&, const int, const int);

	private:
		void StatupUEComputerVisionModule();
		void ShutdownUEComputerVisionModule();

	public:
		UECOMPUTERVISION_API void StartupModule() override;
		UECOMPUTERVISION_API void ShutdownModule() override;

		UECOMPUTERVISION_API float	Run();
		UECOMPUTERVISION_API void	SetPrimaryModel(const FString& modelName);
};
