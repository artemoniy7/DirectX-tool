#include <DXTool/Window/Window.h>

#include <windows.h>

#include <deque>
#include <mutex>
#include <utility>

namespace DXTool
{
    struct Window::Impl
    {
        HWND handle = nullptr;
        bool isOpen = false;
        std::deque<WindowEvent> events;

        static constexpr const wchar_t* ClassName = L"DXToolWindowClass";

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            Impl* self = reinterpret_cast<Impl*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

            if (message == WM_NCCREATE)
            {
                const auto* createInfo = reinterpret_cast<const CREATESTRUCTW*>(lParam);
                self = static_cast<Impl*>(createInfo->lpCreateParams);
                SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
            }

            if (self == nullptr)
                return DefWindowProcW(hwnd, message, wParam, lParam);

            WindowEvent event{};

            switch (message)
            {
            case WM_CLOSE:
                event.type = WindowEventType::Closed;
                self->events.push_back(event);
                self->isOpen = false;
                DestroyWindow(hwnd);
                return 0;

            case WM_DESTROY:
                self->handle = nullptr;
                self->isOpen = false;
                return 0;

            case WM_SIZE:
                event.type = WindowEventType::Resized;
                event.width = static_cast<UInt32>(LOWORD(lParam));
                event.height = static_cast<UInt32>(HIWORD(lParam));
                self->events.push_back(event);
                return 0;

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                event.type = WindowEventType::KeyDown;
                event.keyCode = static_cast<UInt32>(wParam);
                self->events.push_back(event);
                return 0;

            case WM_KEYUP:
            case WM_SYSKEYUP:
                event.type = WindowEventType::KeyUp;
                event.keyCode = static_cast<UInt32>(wParam);
                self->events.push_back(event);
                return 0;

            case WM_MOUSEMOVE:
                event.type = WindowEventType::MouseMoved;
                event.mouseX = GET_X_LPARAM(lParam);
                event.mouseY = GET_Y_LPARAM(lParam);
                self->events.push_back(event);
                return 0;

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
                event.type = WindowEventType::MouseButtonDown;
                event.mouseButton = message == WM_LBUTTONDOWN ? 0u :
                    (message == WM_RBUTTONDOWN ? 1u : 2u);
                event.mouseX = GET_X_LPARAM(lParam);
                event.mouseY = GET_Y_LPARAM(lParam);
                self->events.push_back(event);
                return 0;

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
                event.type = WindowEventType::MouseButtonUp;
                event.mouseButton = message == WM_LBUTTONUP ? 0u :
                    (message == WM_RBUTTONUP ? 1u : 2u);
                event.mouseX = GET_X_LPARAM(lParam);
                event.mouseY = GET_Y_LPARAM(lParam);
                self->events.push_back(event);
                return 0;

            case WM_MOUSEWHEEL:
                event.type = WindowEventType::MouseWheel;
                event.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                self->events.push_back(event);
                return 0;

            default:
                return DefWindowProcW(hwnd, message, wParam, lParam);
            }
        }

        Result<void> Create(const WindowDesc& desc)
        {
            static std::once_flag classRegistration;
            static bool classRegistered = false;

            std::call_once(classRegistration, []
            {
                WNDCLASSEXW windowClass{};
                windowClass.cbSize = sizeof(WNDCLASSEXW);
                windowClass.style = CS_HREDRAW | CS_VREDRAW;
                windowClass.lpfnWndProc = WindowProc;
                windowClass.hInstance = GetModuleHandleW(nullptr);
                windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
                windowClass.lpszClassName = ClassName;

                classRegistered = RegisterClassExW(&windowClass) != 0 ||
                    GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
            });

            if (!classRegistered)
            {
                return Result<void>::Failure(
                    ErrorCode::WindowCreationFailed,
                    "Failed to register the Win32 window class.");
            }

            DWORD style = WS_OVERLAPPEDWINDOW;
            if (!desc.resizable)
                style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

            RECT rect{ 0, 0, static_cast<LONG>(desc.width), static_cast<LONG>(desc.height) };
            if (!AdjustWindowRect(&rect, style, FALSE))
            {
                return Result<void>::Failure(
                    ErrorCode::WindowCreationFailed,
                    "Failed to calculate the window size.");
            }

            handle = CreateWindowExW(
                0,
                ClassName,
                desc.title.c_str(),
                style,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                rect.right - rect.left,
                rect.bottom - rect.top,
                nullptr,
                nullptr,
                GetModuleHandleW(nullptr),
                this);

            if (handle == nullptr)
            {
                return Result<void>::Failure(
                    ErrorCode::WindowCreationFailed,
                    "Failed to create the Win32 window.");
            }

            isOpen = true;

            if (desc.visible)
                ShowWindow(handle, SW_SHOW);

            return Result<void>::Success();
        }
    };

    Window::Window()
        : m_impl(std::make_unique<Impl>())
    {
    }

    Window::~Window()
    {
        if (m_impl && m_impl->handle != nullptr)
            DestroyWindow(m_impl->handle);
    }

    Window::Window(Window&&) noexcept = default;
    Window& Window::operator=(Window&&) noexcept = default;

    Result<std::unique_ptr<Window>> Window::Create(const WindowDesc& desc)
    {
        if (desc.width == 0 || desc.height == 0 || desc.title.empty())
        {
            return Result<std::unique_ptr<Window>>::Failure(
                ErrorCode::InvalidArgument,
                "Window dimensions and title must not be empty.");
        }

        auto window = std::unique_ptr<Window>(new Window());
        auto result = window->Initialize(desc);

        if (!result)
        {
            return Result<std::unique_ptr<Window>>::Failure(result.GetError());
        }

        return Result<std::unique_ptr<Window>>::Success(std::move(window));
    }

    Result<void> Window::Initialize(const WindowDesc& desc)
    {
        return m_impl->Create(desc);
    }

    bool Window::ProcessEvents()
    {
        if (!m_impl || m_impl->handle == nullptr)
            return false;

        MSG message{};
        while (PeekMessageW(&message, m_impl->handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        return m_impl->isOpen;
    }

    bool Window::PollEvent(WindowEvent& event)
    {
        if (!m_impl || m_impl->events.empty())
            return false;

        event = m_impl->events.front();
        m_impl->events.pop_front();
        return true;
    }

    bool Window::IsOpen() const noexcept
    {
        return m_impl && m_impl->isOpen;
    }

    void* Window::GetNativeHandle() const noexcept
    {
        return m_impl ? static_cast<void*>(m_impl->handle) : nullptr;
    }
}
