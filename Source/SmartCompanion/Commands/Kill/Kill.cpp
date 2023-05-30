#include "Kill.h"
#include "../../SmartCompanionCharacter.h"

#include <Kismet/GameplayStatics.h>

Kill::Kill()
{
}

Kill::Kill(ComputerVisionModule& _computerVisionModule) : computerVisionModule(_computerVisionModule)
{
}

void Kill::GeneralRun()
{
	float angle = computerVisionModule.Run();

	auto controller = UGameplayStatics::GetPlayerController(computerVisionModule.GetWorldContext(), 0);
	auto character = (ASmartCompanionCharacter*)(controller->GetPawn());

	const FRotator rotation = character->GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);

	const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	character->AddMovementInput(direction, angle);

	character->OnPrimaryAction();
}
