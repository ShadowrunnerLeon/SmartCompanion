#include "KillBlue.h"

void KillBlue::Run()
{
	computerVisionModule->SetPrimaryModel("blue");
	GeneralRun();
}
