// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../ConfigData.h"
#include "../ISmartModule.h"

#pragma comment(lib, "E:\\SmartCompanion\\ThirdParty\\PortAudio\\Lib\\Release\\portaudio_static_x64.lib")
#pragma comment(lib, "E:\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libvosk.lib")

#include "..\..\ThirdParty\PortAudio\Include\portaudio.h"
#include "..\..\ThirdParty\Vosk\vosk-win64\vosk_api.h"

class UESpeechRecognitionModule : public ISmartModule
{
	private:
		VoskModel* model;
		VoskRecognizer* recognizer;
		PaStream* stream;
		PaStreamParameters inputParametrs;

		char data[SPEECH_BUFFER_SIZE];

		const FString BasePluginDir = "E:/SmartCompanion";

		void* DynamicLibVoskHandle;
		void* DynamicLibPortAudioHandle;

	private:
		bool InializeModelAndRecognizer();
		bool InitializePortAudio();
		bool SetAudioDevice();
		bool OpenStream();
		bool StartStream();
		
		bool ReadDataFromStream();
		std::string Recognize();

		void StartupVosk();
		void StatupPortAudio();

		void ShutdownVosk();
		void ShutdownPortAudio();

	public:
		void StartupModule() override;
		void ShutdownModule() override;

		UESPEECHRECOGNITION_API bool Initialize() override;
		UESPEECHRECOGNITION_API std::string Run();
};
