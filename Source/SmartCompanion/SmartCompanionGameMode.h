// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

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
		TSharedPtr<CommandHandler> commandHandler = MakeShared<CommandHandler>(GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull));

	public:
		ASmartCompanionGameMode();
		void ActivateCommandHandler();
		void DeactivateCommandHandler();
};



