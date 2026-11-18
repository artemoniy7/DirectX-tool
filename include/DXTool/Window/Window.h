#pragma once

#include <DXTool/Core/Result.h>
#include <DXTool/Core/Types.h>
#include <DXTool/Window/WindowEvents.h>

#include <memory>
#include <string>

namespace DXTool
{
    struct WindowDesc
    {
        std::wstring title = L"DXTool Window";
        UInt32 width = 1280;
        UInt32 height = 720;
        bool resizable = true;
        bool visible = true;
    };

    class Window
    {
    public:
        static Result<std::unique_ptr<Window>> Create(const WindowDesc& desc = {});

        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        bool ProcessEvents();
        bool PollEvent(WindowEvent& event);

        bool IsOpen() const noexcept;
        void* GetNativeHandle() const noexcept;

    private:
        struct Impl;

        Window();
        Result<void> Initialize(const WindowDesc& desc);

        std::unique_ptr<Impl> m_impl;
    };
}
