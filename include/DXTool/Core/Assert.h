#pragma once

namespace DXTool::Core
{
    void AssertFailure(
        const char* expression,
        const char* message,
        const char* file,
        int line
    );
}

#ifndef NDEBUG
    #define DXTOOL_ASSERT(expression, message) \
        ((expression) ? static_cast<void>(0) : \
            ::DXTool::Core::AssertFailure(#expression, message, __FILE__, __LINE__))
#else
    #define DXTOOL_ASSERT(expression, message) static_cast<void>(0)
#endif
