#include "Kill.h"
#include "../../SmartCompanionCharacter.h"
#include "../../SmartCompanionGameMode.h"

#include <Kismet/GameplayStatics.h>
#include <Modules/ModuleManager.h>

void Kill::GeneralRun()
{
	auto computerVisionModule = FModuleManager::Get().LoadModuleChecked(TEXT("UEComputerVision"));
	auto controller = UGameplayStatics::GetPlayerController(computerVisionModule->GetWorldContext(), 0);
	auto character = (ASmartCompanionCharacter*)(controller->GetPawn());

	character->ActivateFirstPersonView();
	float angle = computerVisionModule->Run();

	auto gameMode = (computerVisionModule->GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>());

	gameMode->PushTaskInQueue([=]() { character->RotateOnAngleYaw(angle); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
	gameMode->PushTaskInQueue([=]() { character->OnPrimaryAction(); });
}
