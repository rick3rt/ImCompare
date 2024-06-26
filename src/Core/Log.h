#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
  public:
    static void Init();
    static void SetLogLevel(spdlog::level::level_enum level) { s_Logger->set_level(level); }
    inline static std::shared_ptr<spdlog::logger> &GetLogger() { return s_Logger; }

  private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

// Core log macros
#define LOG_TRACE(...) ::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::Log::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) ::Log::GetLogger()->fatal(__VA_ARGS__)
