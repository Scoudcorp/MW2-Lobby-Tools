#include "stdafx.h"
#include "Logger.h"
#include <iostream>

Logger::LOGGER_LEVEL Logger::level = LOGGER_LEVEL_NONE;

void Logger::InitLogger(LOGGER_LEVEL level)
{
	Logger::level = level;
}

void Logger::Log(std::string out, LOGGER_TYPE type)
{
	switch (type)
	{
	case LOGGER_WARNING:
		if (Logger::level >= Logger::LOGGER_LEVEL_WARNING)
			std::cout << out << std::endl;
		break;
	case LOGGER_ERROR:
		if (Logger::level >= Logger::LOGGER_LEVEL_ERROR)
			std::cout << out << std::endl;
		break;
	case LOGGER_DEFAULT:
	case LOGGER_DEBUG:
	default:
		if (Logger::level == Logger::LOGGER_LEVEL_DEBUG ||
			Logger::level == Logger::LOGGER_LEVEL_ALL)
			std::cout << out << std::endl;
	}
}

void Logger::Log(std::string out)
{
	Logger::Log(out, Logger::LOGGER_DEBUG);
}

void Logger::LogDebug(std::string out)
{
	Logger::Log(out, Logger::LOGGER_DEBUG);
}

void Logger::LogWarning(std::string out)
{
	Logger::Log(out, Logger::LOGGER_WARNING);
}

void Logger::LogError(std::string out)
{
	Logger::Log(out, Logger::LOGGER_ERROR);
}
