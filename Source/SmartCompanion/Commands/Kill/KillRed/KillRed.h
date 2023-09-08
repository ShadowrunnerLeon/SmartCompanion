#pragma once

#include "../../ICommand.h"
#include "../Kill.h"

class KillRed : public ICommand, public Kill
{
	public:
		KillRed() = default;
		void Run() override;
};