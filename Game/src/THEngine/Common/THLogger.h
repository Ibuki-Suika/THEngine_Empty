#ifndef THLOGGER_H
#define THLOGGER_H

#include <fstream>
#include "THString.h"

namespace THEngine
{
	class Logger
	{
	private:
		Logger();
		

	private:
		static Logger* instance;
		std::ofstream logFile;

	public:
		~Logger();
		static Logger* GetInstance();
		void WriteLine(String message);
	};
}

#endif