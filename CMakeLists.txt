cmake_minimum_required(VERSION 3.20)

project(DXTool
    VERSION 0.1.0
    DESCRIPTION "A hybrid C++17 framework built on Direct3D 12"
    LANGUAGES CXX
)

if(NOT WIN32)
    message(FATAL_ERROR "DXTool currently supports Windows only.")
endif()

add_library(DXTool STATIC
    src/Core/Logger.cpp
    src/Core/Assert.cpp
    src/Window/Window.cpp
    src/DXTool.cpp
)

add_library(DXTool::DXTool ALIAS DXTool)

target_compile_features(DXTool PUBLIC cxx_std_17)

target_include_directories(DXTool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(DXTool PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX)

target_link_libraries(DXTool PUBLIC d3d12 dxgi dxguid)

if(MSVC)
    target_compile_options(DXTool PRIVATE /W4 /permissive-)
endif()

option(DXTOOL_BUILD_EXAMPLES "Build DXTool examples" OFF)
option(DXTOOL_BUILD_TESTS "Build DXTool tests" OFF)

if(DXTOOL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(DXTOOL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
