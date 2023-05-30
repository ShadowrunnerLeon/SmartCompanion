#include "KillRed.h"

KillRed::KillRed()
{
}

KillRed::KillRed(ComputerVisionModule& _computerVisionModule) : Kill(_computerVisionModule)
{
}

void KillRed::Run()
{
	computerVisionModule.SetPrimaryModel("red");
	GeneralRun();
}
