#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "../ICommand.h"
#include "../../SmartModules/SpeechRecognitionModule/SpeechRecognitionModule.h"
#include "../../SmartModules/ComputerVisionModule/ComputerVisionModule.h"

#include <map>
#include <string>
#include <memory>

class CommandHandler : public FRunnable
{
	public:
		CommandHandler();
		CommandHandler(UWorld* _worldContext);
		~CommandHandler();

		bool Init() override;
		uint32 Run() override;
		void Stop() override;

		void Activate();
		void Deactivate();

	private:
		FRunnableThread* thread;
		bool bRunThread;

		bool isActivateSpeechRecognition;

		std::map<std::string, std::shared_ptr<ICommand>> commandStorage;
		SpeechRecognitionModule speechRecoginitonModule;
		ComputerVisionModule computerVisionModule;

		UWorld* worldContext;
};