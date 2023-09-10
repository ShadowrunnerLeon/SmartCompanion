#include "KillRed.h"
#include "../../../../UEComputerVision/UEComputerVisionModule.h"

#include <Modules/ModuleManager.h>

void KillRed::Run()
{
	COMPUTERVISION_SINGLETON.SetPrimaryModel("red");
	GeneralRun();
}
