#include "HBEpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace HBestEngine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// Set the global pattern, check https://github.com/gabime/spdlog/wiki/3.-Custom-formatting for more information about formatting
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("HBE");
		// Only messages whose level >= log_level will be logged to the console, in this case, all types of messages will be logged
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}