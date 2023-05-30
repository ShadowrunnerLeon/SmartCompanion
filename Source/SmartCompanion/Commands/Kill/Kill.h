#pragma once

#include "../../SmartModules/ComputerVisionModule/ComputerVisionModule.h"

class Kill
{
	protected:
		ComputerVisionModule computerVisionModule;
	
	public:
		Kill();
		Kill(ComputerVisionModule& _computerVisionModule);

		void GeneralRun();
};