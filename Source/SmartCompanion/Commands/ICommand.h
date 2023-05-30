#pragma once

class ICommand
{
	public:
		virtual void Run() = 0;
		virtual ~ICommand() {};
};