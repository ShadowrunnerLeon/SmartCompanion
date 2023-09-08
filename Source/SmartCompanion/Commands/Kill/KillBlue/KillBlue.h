#pragma once

#include "../../ICommand.h"
#include "../Kill.h"

class KillBlue : public ICommand, public Kill
{
	public:
		KillBlue() = default;
		void Run() override;
};