#include <DXTool/Core/Logger.h>

#include <iostream>
#include <mutex>

namespace
{
    std::mutex g_loggerMutex;
    DXTool::Core::LogLevel g_minimumLevel = DXTool::Core::LogLevel::Debug;

    const char* GetLevelName(DXTool::Core::LogLevel level)
    {
        using DXTool::Core::LogLevel;

        switch (level)
        {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
        default:                return "UNKNOWN";
        }
    }
}

namespace DXTool::Core
{
    void Logger::SetMinimumLevel(LogLevel level) noexcept
    {
        std::lock_guard<std::mutex> lock(g_loggerMutex);
        g_minimumLevel = level;
    }

    void Logger::Write(LogLevel level, std::string_view message)
    {
        std::lock_guard<std::mutex> lock(g_loggerMutex);

        if (level < g_minimumLevel)
            return;

        std::ostream& output =
            level == LogLevel::Error ? std::cerr : std::clog;

        output << "[DXTool][" << GetLevelName(level) << "] "
               << message << '\n';
    }

    void Logger::Debug(std::string_view message)
    {
        Write(LogLevel::Debug, message);
    }

    void Logger::Info(std::string_view message)
    {
        Write(LogLevel::Info, message);
    }

    void Logger::Warning(std::string_view message)
    {
        Write(LogLevel::Warning, message);
    }

    void Logger::Error(std::string_view message)
    {
        Write(LogLevel::Error, message);
    }
}
