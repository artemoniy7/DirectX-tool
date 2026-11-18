# DXTool

**DXTool** is a C++17 framework built on **Direct3D 12**. Its goal is to make common graphics and application tasks easier without hiding the native DirectX API.

## Project direction

DXTool follows a hybrid architecture:

- A high-level API for common application and rendering tasks.
- Low-level access to native Direct3D 12 and DXGI objects when needed.
- Clear ownership and lifetime rules using RAII.
- A modular structure that can grow without becoming a full game engine.

## Requirements

- Windows 10 or newer
- Visual Studio 2022 with C++ desktop development tools
- Windows SDK with Direct3D 12 headers and libraries
- CMake 3.20 or newer

## Build

    cmake -S . -B build
    cmake --build build --config Debug

## Use from another CMake project

    add_subdirectory(external/DirectX-tool)
    target_link_libraries(MyApp PRIVATE DXTool::DXTool)

## Current status

The repository currently contains the initial project skeleton. Core systems and the public API will be added incrementally.

## License

License has not been selected yet.
