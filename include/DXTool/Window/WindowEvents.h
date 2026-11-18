#pragma once

#include <DXTool/Core/Types.h>

namespace DXTool
{
    enum class WindowEventType
    {
        Closed,
        Resized,
        KeyDown,
        KeyUp,
        MouseMoved,
        MouseButtonDown,
        MouseButtonUp,
        MouseWheel
    };

    struct WindowEvent
    {
        WindowEventType type = WindowEventType::Closed;

        UInt32 width = 0;
        UInt32 height = 0;

        UInt32 keyCode = 0;
        UInt32 mouseButton = 0;

        Int32 mouseX = 0;
        Int32 mouseY = 0;
        Int32 wheelDelta = 0;
    };
}
