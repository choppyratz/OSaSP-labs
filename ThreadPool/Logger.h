#pragma once
#include <iostream>

using namespace std;

class Logger
{
	public:
		void info(string text);
		Logger();
		virtual ~Logger();
	private: 
		FILE *fHandle;
};

