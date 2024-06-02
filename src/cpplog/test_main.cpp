#include <iostream>
using namespace std;
#include "Log.h"

int main(int paramCount, char* paramList[])
{

	init_log_cpp("main-log.txt", Log::LOG_LEVEL::DEBUG_LEVEL, Log::LOG_TARGET::ALL, "datetime_level_func_log_source_line");
	for (size_t i = 0; i < paramCount; i++)
	{
		infor_log_cpp("param:[" << paramList[i] << "]");
	}

	debug_log_cpp("Hello World!");
	error_log_cpp("I done this work for " << 32 << " hours.");
	return 0;
}
