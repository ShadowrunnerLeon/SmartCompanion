// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class SmartCompanionModule : public FDefaultGameModuleImpl
{
	public:
		virtual void StartupModule() override;
		virtual void ShutdownModule() override;

		void* DynamicLibVoskHandle;
		void* DynamicLibPortAudioHandle;
		void* DynamicLibOpenCVHandle;

	private:
		const FString BasePluginDir = "D:/SmartCompanion/SmartCompanion";

	private:
		void StartupVosk();
		void StatupPortAudio();
		void StatupOpenCV();

		void ShutdownVosk();
		void ShutdownPortAudio();
		void ShutdownOpenCV();
};
