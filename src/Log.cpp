#include "Log.h"

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init()
{
    // spdlog::set_pattern("%^[%T] %n: %v%$");
    spdlog::set_pattern("[%H:%M:%S][%n][%l] %v");

    s_Logger = spdlog::stdout_color_mt("DataExplorer");
    s_Logger->set_level(spdlog::level::trace);
}