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

	(computerVisionModule->GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().push([=]() { character->RotateOnAngleYaw(angle); });
	(computerVisionModule->GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().push([=]() { character->OnPrimaryAction(); });
	(computerVisionModule->GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().push([=]() { character->OnPrimaryAction(); });
	(computerVisionModule->GetWorldContext()->GetAuthGameMode<ASmartCompanionGameMode>())->GetTaskQueue().push([=]() { character->OnPrimaryAction(); });
}
