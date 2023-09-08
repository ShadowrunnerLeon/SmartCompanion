#include "KillRed.h"

void KillRed::Run()
{
	computerVisionModule->SetPrimaryModel("red");
	GeneralRun();
}
