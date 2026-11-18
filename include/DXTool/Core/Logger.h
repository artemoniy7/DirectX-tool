#pragma once

#include <string_view>

namespace DXTool::Core
{
    enum class LogLevel
    {
        Debug = 0,
        Info,
        Warning,
        Error
    };

    class Logger
    {
    public:
        static void SetMinimumLevel(LogLevel level) noexcept;
        static void Write(LogLevel level, std::string_view message);

        static void Debug(std::string_view message);
        static void Info(std::string_view message);
        static void Warning(std::string_view message);
        static void Error(std::string_view message);
    };
}
