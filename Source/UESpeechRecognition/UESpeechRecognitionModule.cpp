// Copyright Epic Games, Inc. All Rights Reserved.

#include "UESpeechRecognitionModule.h"
#include "simpleson/json.h"
#include "Modules/ModuleManager.h"

void UESpeechRecognitionModule::StartupVosk()
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

void UESpeechRecognitionModule::StatupPortAudio()
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

void UESpeechRecognitionModule::ShutdownVosk()
{
	if (DynamicLibVoskHandle) FPlatformProcess::FreeDllHandle(DynamicLibVoskHandle);
	DynamicLibVoskHandle = nullptr;
}

void UESpeechRecognitionModule::ShutdownPortAudio()
{
	if (DynamicLibPortAudioHandle) FPlatformProcess::FreeDllHandle(DynamicLibPortAudioHandle);
	DynamicLibPortAudioHandle = nullptr;
}

void UESpeechRecognitionModule::StartupModule()
{
	StartupVosk();
	StatupPortAudio();
}

void UESpeechRecognitionModule::ShutdownModule()
{
	Pa_CloseStream(stream);
	vosk_recognizer_free(recognizer);
	vosk_model_free(model);

	if (DynamicLibVoskHandle) ShutdownVosk();
	if (DynamicLibPortAudioHandle) ShutdownPortAudio();
}

bool UESpeechRecognitionModule::Initialize()
{
	if (!InializeModelAndRecognizer()) return false;
	if (!InitializePortAudio()) return false;
	if (!SetAudioDevice()) return false;
	if (!OpenStream()) return false;
	if (!StartStream()) return false;

	return true;
}

bool UESpeechRecognitionModule::InializeModelAndRecognizer()
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

bool UESpeechRecognitionModule::InitializePortAudio()
{
	PaError err = Pa_Initialize();
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_Initialize: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

bool UESpeechRecognitionModule::SetAudioDevice()
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

bool UESpeechRecognitionModule::OpenStream()
{
	PaError err = Pa_OpenStream(&stream, &inputParametrs, nullptr, 16000.0, 8192, 0, nullptr, nullptr);
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_OpenStream: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

bool UESpeechRecognitionModule::StartStream()
{
	PaError err = Pa_StartStream(stream);
	if (err != paNoError)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_StartStream: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

std::string UESpeechRecognitionModule::Run()
{
	bool isCorrectRead = ReadDataFromStream();
	if (!isCorrectRead) return {};

	std::string recognizedText = Recognize();
	return recognizedText;
}

bool UESpeechRecognitionModule::ReadDataFromStream()
{
	PaError err = Pa_ReadStream(stream, (void*)data, SPEECH_BUFFER_SIZE / 2);
	if (err != paNoError && err != paInputOverflowed)
	{
		UE_LOG(LogTemp, Display, TEXT("Pa_ReadStream: "), Pa_GetErrorText(err));
		return false;
	}

	return true;
}

std::string UESpeechRecognitionModule::Recognize()
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

IMPLEMENT_MODULE(UESpeechRecognitionModule, UESpeechRecognition);
 