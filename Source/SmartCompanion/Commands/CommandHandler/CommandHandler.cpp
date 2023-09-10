#include "CommandHandler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "../../SmartCompanionCharacter.h"
#include "../Kill/KillRed/KillRed.h"
#include "../Kill/KillBlue/KillBlue.h"

#pragma region Main Thread Code

CommandHandler::CommandHandler()
{
	isActivateSpeechRecognition = false;
	thread = FRunnableThread::Create(this, TEXT("CommandHandlerThread"));
}

CommandHandler::CommandHandler(UWorld* _worldContext) :
	worldContext(_worldContext)
{
	SPEECHRECOGNITION_SINGLETON.SetWorldContext(_worldContext);
	COMPUTERVISION_SINGLETON.SetWorldContext(_worldContext);

	isActivateSpeechRecognition = false;
	thread = FRunnableThread::Create(this, TEXT("CommandHandlerThread"));
}

CommandHandler::~CommandHandler()
{
	if (thread)
	{
		thread->Kill();
		delete thread;
	}
}

#pragma endregion

bool CommandHandler::Init()
{
	UE_LOG(LogTemp, Display, TEXT("CommandHandler initialize"));

	commandStorage.Add(FString("red"), TSharedPtr<ICommand>(new KillRed()));
	commandStorage.Add(FString("read"), TSharedPtr<ICommand>(new KillRed()));
	commandStorage.Add(FString("blue"), TSharedPtr<ICommand>(new KillBlue()));

	SPEECHRECOGNITION_SINGLETON.Initialize();
	COMPUTERVISION_SINGLETON.Initialize();

	bRunThread = true;
	return true;
}

uint32 CommandHandler::Run()
{
	while (bRunThread)
	{
		if (isActivateSpeechRecognition)
		{
			//FindCommand();
			commandStorage[FString("blue")]->Run();
			isActivateSpeechRecognition = false;
		}
	}

	return 0;
}

void CommandHandler::Stop()
{
	bRunThread = false;
}

void CommandHandler::FindCommand()
{
	auto text = speechRecoginitonModule.Run();

	for (auto& [key, value] : commandStorage)
	{
		auto convertedKey = TCHAR_TO_UTF8(*key);
		auto pos = text.find(convertedKey);
		if (pos != std::string::npos)
		{
			value->Run();
			isActivateSpeechRecognition = false;
			break;
		}
	}
}

void CommandHandler::Activate()
{
	isActivateSpeechRecognition = true;
}

void CommandHandler::Deactivate()
{
	isActivateSpeechRecognition = false;
}
