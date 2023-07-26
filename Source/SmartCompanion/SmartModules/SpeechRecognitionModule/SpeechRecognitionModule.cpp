#include "SpeechRecognitionModule.h"
#include "simpleson/json.h"
#include <Logging/LogMacros.h>

SpeechRecognitionModule::SpeechRecognitionModule()
{
}

SpeechRecognitionModule::SpeechRecognitionModule(UWorld* _worldContext)
{
	worldContext = _worldContext;
}

bool SpeechRecognitionModule::Initialize()
{
	if (!InializeModelAndRecognizer()) return false;
	if (!InitializePortAudio()) return false;
	if (!SetAudioDevice()) return false;
	if (!OpenStream()) return false;
	if (!StartStream()) return false;

	return true;
}

bool SpeechRecognitionModule::InializeModelAndRecognizer()
{
	std::string path = baseDir + "\\Models\\Vosk\\vosk-model-small-en-us-0.15";
	
	model = vosk_model_new(path.c_str());
	if (!model)
	{
		UE_LOG(LogTemp, Display, TEXT("vosk_model_new: error"));
		return false;
	}

	recognizer = vosk_recognizer_new(model, 16000.0);
	if (!recognizer)
	{
		UE_LOG(LogTemp, Display, TEXT("vosk_recognizer_new: error"));
		return false;
	}

	return true;
}

bool SpeechRecognitionModule::InitializePortAudio()
{
	PaError err = Pa_Initialize();
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_Initialize: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

bool SpeechRecognitionModule::SetAudioDevice()
{
	inputParametrs.channelCount = 1;
	inputParametrs.sampleFormat = paInt16;
	inputParametrs.hostApiSpecificStreamInfo = nullptr;
	inputParametrs.device = Pa_GetDefaultInputDevice();

	if (inputParametrs.device == paNoDevice)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_GetDefaultInputDevice: no device"));
		return false;
	}

	return true;
}

bool SpeechRecognitionModule::OpenStream()
{
	PaError err = Pa_OpenStream(&stream, &inputParametrs, nullptr, 16000.0, 8192, 0, nullptr, nullptr);
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_OpenStream: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

bool SpeechRecognitionModule::StartStream()
{
	PaError err = Pa_StartStream(stream);
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_StartStream: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

std::string SpeechRecognitionModule::Run()
{
	bool isCorrectRead = ReadDataFromStream();
	if (!isCorrectRead) return {};

	std::string recognizedText = Recognize();
	return recognizedText;
}

bool SpeechRecognitionModule::ReadDataFromStream()
{
	PaError err = Pa_ReadStream(stream, (void*)data, SPEECH_BUFFER_SIZE / 2);
	if (err != paNoError && err != paInputOverflowed)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_ReadStream: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

std::string SpeechRecognitionModule::Recognize()
{
	if (vosk_recognizer_accept_waveform(recognizer, data, sizeof(data)) == -1)
	{
		UE_LOG(LogTemp, Display, TEXT("vosk_recognizer_accept_waveform: error"));
		return {};
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
