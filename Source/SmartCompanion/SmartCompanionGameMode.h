// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <queue>
#include <functional>

#include "Commands/CommandHandler/CommandHandler.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "SmartCompanionGameMode.generated.h"

UCLASS(minimalapi)
class ASmartCompanionGameMode : public AGameModeBase
{
	GENERATED_BODY()

	private:
		std::queue<std::function<void()>> taskQueue;
		TSharedPtr<CommandHandler> commandHandler;

	public:
		ASmartCompanionGameMode();
		void ActivateCommandHandler();
		void DeactivateCommandHandler();

		std::queue<std::function<void()>>& GetTaskQueue();
};



