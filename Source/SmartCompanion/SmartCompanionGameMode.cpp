// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmartCompanionGameMode.h"
#include "SmartCompanionCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASmartCompanionGameMode::ASmartCompanionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}