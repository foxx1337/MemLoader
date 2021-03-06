﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("MemLoader")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

enable_testing()

# Include sub-projects.
add_subdirectory("MemoryFunctions")
add_subdirectory("TimedExecutor")
add_subdirectory("CopyCompareBench")
if (BUILD_EXPERIMENTS)
    add_subdirectory("experiments")
endif()

set(cpp_files_core
    "src/pattern.cpp"
    "src/hot_spot.cpp"
    "src/pass.cpp"
    "src/memory_helpers.cpp"
)

set(h_files_core
    "src/pattern.h"
    "src/hot_spot.h"
    "src/pass.h"
    "src/memory_helpers.h"
)

set(cpp_files_gui
    "src/MemLoader.cpp"
    "src/forms/mainwindow.cpp"
)

set(h_files_gui
    "src/forms/mainwindow.h"
)

set(ui_files
    "src/forms/mainwindow.ui"
)

set(resource_files
    "src/forms/MemLoader.qrc"
)


if(WIN32)
    list(APPEND cpp_files_core "src/win/platform_memory.cpp")
elseif(UNIX)
    list(APPEND cpp_files_core "src/posix/platform_memory.cpp")
endif()

add_executable(MemLoaderCliBench "src/MemLoaderCliBench.cpp" ${cpp_files_core} ${h_files_core})

find_package(Threads)

target_link_libraries(MemLoaderCliBench
    MemoryFunctions
    TimedExecutor
    ${CMAKE_THREAD_LIBS_INIT}
)

if (BUILD_MEMLOADER_GUI)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_executable(MemLoader
        ${cpp_files_gui}
        ${cpp_files_core}
        ${h_files_gui}
        ${h_files_core}
        ${ui_files}
        ${resource_files}
    )

    target_link_libraries(MemLoader
        PRIVATE Qt5::Widgets
        MemoryFunctions
        TimedExecutor
        ${CMAKE_THREAD_LIBS_INIT}
    )
endif()
