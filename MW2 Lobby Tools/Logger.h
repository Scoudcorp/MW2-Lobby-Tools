#pragma once

class Logger
{
public:
	enum LOGGER_LEVEL
	{
		LOGGER_LEVEL_NONE,		// Do not log
		LOGGER_LEVEL_DEBUG,		// Log all debug
		LOGGER_LEVEL_WARNING,	// Log all warning
		LOGGER_LEVEL_ERROR,		// Log all warning + error
		LOGGER_LEVEL_ALL		// Log all  debug + warning + error
	};

	enum LOGGER_TYPE
	{
		LOGGER_DEFAULT,		// Default option is same as debug
		LOGGER_DEBUG,		// Usefull debug information
		LOGGER_WARNING,		// Warning can indicate something is wrong
		LOGGER_ERROR		// Fatal errors stop the program
	};

	static void InitLogger(LOGGER_LEVEL level);

	static void Log(std::string out, LOGGER_TYPE type);
	static void Log(std::string out);

	static void LogDebug(std::string out);
	static void LogWarning(std::string out);
	static void LogError(std::string out);

private:
	static LOGGER_LEVEL level;
};
