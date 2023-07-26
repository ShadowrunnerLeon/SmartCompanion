#pragma once

#include "../../ConfigData.h"
#include "../ISmartModule.h"

#pragma comment(lib, "E:\\SmartCompanion\\ThirdParty\\PortAudio\\Lib\\Release\\portaudio_static_x64.lib")
#pragma comment(lib, "E:\\SmartCompanion\\ThirdParty\\Vosk\\vosk-win64\\libvosk.lib")

#include "..\..\..\..\ThirdParty\PortAudio\Include\portaudio.h"
#include "..\..\..\..\ThirdParty\Vosk\vosk-win64\vosk_api.h"

class SpeechRecognitionModule : public ISmartModule
{
	private:
		VoskModel* model;
		VoskRecognizer* recognizer;
		PaStream* stream;
		PaStreamParameters inputParametrs;

		char data[SPEECH_BUFFER_SIZE];

	private:
		bool InializeModelAndRecognizer();
		bool InitializePortAudio();
		bool SetAudioDevice();
		bool OpenStream();
		bool StartStream();
		
		bool ReadDataFromStream();
		std::string Recognize();

	public:
		SpeechRecognitionModule();
		SpeechRecognitionModule(UWorld* _worldContext);

		bool		Initialize()	override;
		void		Shutdown()		override;

		std::string Run();
};