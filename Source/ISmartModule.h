#pragma once

#include "CoreMinimal.h"

class ISmartModule : public FDefaultGameModuleImpl
{
	protected:
		UWorld* worldContext;

	public:
		template <typename T>
		T Run() = 0;

		void SetWorldContext(UWorld* _worldContext) { worldContext = _worldContext; }
		UWorld* GetWorldContext() { return worldContext; }

		virtual ~ISmartModule() {};
};