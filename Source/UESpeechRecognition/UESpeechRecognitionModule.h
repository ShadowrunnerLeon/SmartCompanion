// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../ConfigData.h"
#include "../ISmartModule.h"

#define SPEECHRECOGNITION_SINGLETON 	((UESpeechRecognitionModule&)(FModuleManager::Get().LoadModuleChecked(TEXT("UESpeechRecognition"))))

class UESpeechRecognitionModule : public ISmartModule
{
	private:
		void* DynamicLibSpeechRecognitionModuleHandle;

	private:
		const char* (*Initialize)();
    	void (*Shutdown)();

		void StartupUESpeechRecognitionModule();
		void ShutdownUESpeechRecognitionModule();

	public:
		UESPEECHRECOGNITION_API void StartupModule() override;
		UESPEECHRECOGNITION_API void ShutdownModule() override;

		const char* (*Run)();
};
