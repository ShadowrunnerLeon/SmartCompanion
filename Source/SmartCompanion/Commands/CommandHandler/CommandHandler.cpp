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
	worldContext(_worldContext),
	speechRecoginitonModule(_worldContext),
	computerVisionModule(_worldContext)
{
	isActivateSpeechRecognition = false;
	thread = FRunnableThread::Create(this, TEXT("CommandHandlerThread"));
}

CommandHandler::~CommandHandler()
{
	speechRecoginitonModule.Shutdown();

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

	commandStorage["read"] = std::shared_ptr<ICommand>(new KillRed(computerVisionModule));
	commandStorage["blue"] = std::shared_ptr<ICommand>(new KillBlue(computerVisionModule));

	speechRecoginitonModule.Initialize();
	computerVisionModule.Initialize();

	return true;
}

uint32 CommandHandler::Run()
{
	while (bRunThread)
	{
		if (isActivateSpeechRecognition)
		{
			auto text = speechRecoginitonModule.Run();

			for (auto& [key, value] : commandStorage)
			{
				auto pos = text.find(key);
				if (pos != std::string::npos)
				{
					value->Run();
					break;
				}
			}
		}
	}

	return 0;
}

void CommandHandler::Stop()
{
	bRunThread = false;
}

void CommandHandler::Activate()
{
	isActivateSpeechRecognition = true;
}

void CommandHandler::Deactivate()
{
	isActivateSpeechRecognition = false;
}
