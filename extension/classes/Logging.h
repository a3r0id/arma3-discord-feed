#pragma once

class Logging
{
public:
	static void baseLog(const char* message, const char* file);
	static void logDebug(const char* message);
	static void logError(const char* message);
};