#include "Kill.h"
#include "../../SmartCompanionCharacter.h"
#include "../../SmartCompanionGameMode.h"

#include <Kismet/GameplayStatics.h>

Kill::Kill()
{
}

Kill::Kill(ComputerVisionModule* _computerVisionModule) : computerVisionModule(_computerVisionModule)
{
}

void Kill::GeneralRun()
{
	float angle = computerVisionModule->Run();

	auto controller = UGameplayStatics::GetPlayerController(computerVisionModule->GetWorldContext(), 0);
	auto character = (ASmartCompanionCharacter*)(controller->GetPawn());

	auto gameMode = (computerVisionModule->GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>());

	gameMode->PushTaskInQueue([=]() { character->RotateOnAngleYaw(angle); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
}
