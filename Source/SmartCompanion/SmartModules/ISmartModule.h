#pragma once

class ISmartModule
{
	protected:
		UWorld* worldContext;

	public:
		virtual bool Initialize() = 0;

		template <typename T>
		T Run() = 0;

		virtual void Shutdown() = 0;
		virtual ~ISmartModule() {};
};