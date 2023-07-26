// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmartCompanionGameMode.h"
#include "SmartCompanionCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASmartCompanionGameMode::ASmartCompanionGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (!PlayerPawnBPClass.Class) return;
	DefaultPawnClass = PlayerPawnBPClass.Class;
	commandHandler = MakeShared<CommandHandler>(GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull));
}

void ASmartCompanionGameMode::ActivateCommandHandler()
{
	commandHandler->Activate();
}

void ASmartCompanionGameMode::DeactivateCommandHandler()
{
	commandHandler->Deactivate();
}

void ASmartCompanionGameMode::PushTaskInQueue(std::function<void()> func)
{
	taskQueue.push(func);
}

void ASmartCompanionGameMode::UseFirstTaskFromQueue()
{
	if (!taskQueue.empty())
	{
		(taskQueue.front())();
		taskQueue.pop();
	}
}