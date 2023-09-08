#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "../ICommand.h"
#include "../../../UESpeechRecognition/SpeechRecognitionModule.h"
#include "../../../UEComputerVision/ComputerVisionModule.h"

#include "Templates/SharedPointer.h"
#include <string>

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

		//std::map<std::string, std::shared_ptr<ICommand>> commandStorage;
		TMap<FString, TSharedPtr<ICommand>> commandStorage;

		SpeechRecognitionModule speechRecoginitonModule;
		ComputerVisionModule computerVisionModule;

		UWorld* worldContext;
};