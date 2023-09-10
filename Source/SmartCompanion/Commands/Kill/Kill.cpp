#include "Kill.h"
#include "../../SmartCompanionCharacter.h"
#include "../../SmartCompanionGameMode.h"

#include <Kismet/GameplayStatics.h>
#include <Modules/ModuleManager.h>

#include "../../../UEComputerVision/UEComputerVisionModule.h"

void Kill::GeneralRun()
{
	auto controller = UGameplayStatics::GetPlayerController(COMPUTERVISION_SINGLETON.GetWorldContext(), 0);
	auto character = (ASmartCompanionCharacter*)(controller->GetPawn());

	character->ActivateFirstPersonView();
	float angle = COMPUTERVISION_SINGLETON.Run();

	auto gameMode = (COMPUTERVISION_SINGLETON.GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>());

	gameMode->PushTaskInQueue([=]() { character->RotateOnAngleYaw(angle); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
}
