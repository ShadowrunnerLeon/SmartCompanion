#include "SpeechRecognitionModule.h"
#include "simpleson/json.h"
#include <Logging/LogMacros.h>

// simpleson ������� �� ThirdParty ��-�� ������ ��������

SpeechRecognitionModule::SpeechRecognitionModule()
{
}

SpeechRecognitionModule::SpeechRecognitionModule(UWorld* _worldContext)
{
	worldContext = _worldContext;
}

void SpeechRecognitionModule::Initialize()
{
	InializeModelAndRecognizer();
	InitializePortAudio();
	SetAudioDevice();
	OpenStream();
	StartStream();
}

void SpeechRecognitionModule::InializeModelAndRecognizer()
{
	std::string path = baseDir + "\\Models\\Vosk\\vosk-model-small-en-us-0.15";
	model = vosk_model_new(path.c_str());
	recognizer = vosk_recognizer_new(model, 16000.0);
}

void SpeechRecognitionModule::InitializePortAudio()
{
	PaError err = Pa_Initialize();
	if (Pa_Initialize() != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_Initialize: "), Pa_GetErrorText(err));
		return;
	}
}

void SpeechRecognitionModule::SetAudioDevice()
{
	PaStreamParameters inputParametrs;
	inputParametrs.channelCount = 1;
	inputParametrs.sampleFormat = paInt16;
	inputParametrs.hostApiSpecificStreamInfo = nullptr;
	inputParametrs.device = Pa_GetDefaultInputDevice();

	if (inputParametrs.device == paNoDevice)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_GetDefaultInputDevice: no device"));
		return;
	}
}

void SpeechRecognitionModule::OpenStream()
{
	PaError err = Pa_OpenStream(&stream, &inputParametrs, nullptr, 16000.0, 8192, 0, nullptr, nullptr);
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_OpenStream: "), Pa_GetErrorText(err));
		return;
	}
}

void SpeechRecognitionModule::StartStream()
{
	PaError err = Pa_StartStream(stream);
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_StartStream: "), Pa_GetErrorText(err));
		return;
	}
}

std::string SpeechRecognitionModule::Run()
{
	ReadDataFromStream();
	std::string recognizedText = Recognize();
	return recognizedText;
}

void SpeechRecognitionModule::ReadDataFromStream()
{
	PaError err = Pa_ReadStream(stream, (void*)data, SPEECH_BUFFER_SIZE / 2);
	if (err != paNoError && err != paInputOverflowed)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_ReadStream: "), Pa_GetErrorText(err));
		return nullptr;
	}
}

std::string SpeechRecognitionModule::Recognize()
{
	if (vosk_recognizer_accept_waveform(recognizer, data, sizeof(data)) == -1)
	{
		UE_LOG(LogTemp, Display, TEXT("vosk_recognizer_accept_waveform: error"));
		return nullptr;
	}

	auto resRegonition(vosk_recognizer_result(recognizer));
	auto resJSON = json::jobject::parse(resRegonition);

	FString textFString(resJSON.get("text").c_str());
	UE_LOG(LogTemp, Display, TEXT("TEXT: %s"), *textFString);

	return resJSON.get("text");
}

void SpeechRecognitionModule::Shutdown()
{
	Pa_CloseStream(stream);
	vosk_recognizer_free(recognizer);
	vosk_model_free(model);
}
