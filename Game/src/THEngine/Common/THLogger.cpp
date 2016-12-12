#include <fstream>
#include "THLogger.h"

using namespace THEngine;

Logger* Logger::instance = NULL;

Logger::Logger()
{
	logFile.open("THLog.txt",std::ios::binary);
	char unicodeBom[3] = { (char)0xff, (char)0xfe, (char)0x00 };
	logFile.write(unicodeBom,2);
}

Logger::~Logger()
{
	logFile.close();
}

Logger* Logger::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Logger();
	}
	return instance;
}

void Logger::WriteLine(String message)
{
	logFile.write((char*)message.GetBuffer(), message.GetLength() * 2);
	logFile.write((char*)TEXT("\r\n"),4);
}