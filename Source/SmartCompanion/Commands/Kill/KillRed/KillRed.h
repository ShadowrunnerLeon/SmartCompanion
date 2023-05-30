#pragma once

#include "../../ICommand.h"
#include "../Kill.h"

class KillRed : public ICommand, public Kill
{
	public:
		KillRed();
		KillRed(ComputerVisionModule& _computerVisionModule);

		void Run() override;
};