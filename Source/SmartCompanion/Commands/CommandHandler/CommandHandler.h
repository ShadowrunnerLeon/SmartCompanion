#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include <Modules/ModuleManager.h>
#include <Templates/SharedPointer.h>

#include <string>

#include "../ICommand.h"
#include "../../../UESpeechRecognition/UESpeechRecognitionModule.h"
#include "../../../UEComputerVision/UEComputerVisionModule.h"

class CommandHandler : public FRunnable
{
	public:
		CommandHandler();
		CommandHandler(UWorld* _worldContext);
		~CommandHandler();

		bool Init() override;
		uint32 Run() override;
		void Stop() override;

		void FindCommand();

		void Activate();
		void Deactivate();

	private:
		FRunnableThread* thread;
		bool bRunThread;

		bool isActivateSpeechRecognition;
		TMap<FString, TSharedPtr<ICommand>> commandStorage;

		UESpeechRecognitionModule speechRecoginitonModule;
		UEComputerVisionModule computerVisionModule;

		UWorld* worldContext;
};