#pragma once

#include "CoreMinimal.h"

class ISmartModule : public FDefaultGameModuleImpl
{
	protected:
		UWorld* worldContext;

	public:
		virtual bool Initialize() = 0;
		template <typename T>
		T Run() = 0;
		virtual ~ISmartModule() {};
};