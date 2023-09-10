#include "KillBlue.h"
#include "../../../../UEComputerVision/UEComputerVisionModule.h"

#include <Modules/ModuleManager.h>

void KillBlue::Run()
{
	COMPUTERVISION_SINGLETON.SetPrimaryModel("blue");
	GeneralRun();
}
