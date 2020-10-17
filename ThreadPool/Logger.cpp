#include "Logger.h"


Logger::Logger() {
	fopen_s(&(Logger::fHandle), "info.log", "a+t");
}

void Logger::info(string text) {
	string tempStr = text + "\n";
	fputs(tempStr.c_str(), Logger::fHandle);
}

Logger::~Logger()
{
	fclose(Logger::fHandle);
}