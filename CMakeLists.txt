# CMakeList.txt : Top-level CMake project file, do global configuration
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

set(cpp_files
    "src/MemLoader.cpp"
    "src/pattern.cpp"
    "src/hot_spot.cpp"
    "src/pass.cpp"
    "src/memory_helpers.cpp"
)

set(h_files
    "src/pattern.h"
    "src/hot_spot.h"
    "src/pass.h"
    "src/memory_helpers.h"
)

if(WIN32)
    list(APPEND cpp_files "src/win/platform_memory.cpp")
elseif(UNIX)
    list(APPEND cpp_files "src/posix/platform_memory.cpp")
endif()

add_executable(MemLoader ${cpp_files} ${h_files})

find_package(Threads)
target_link_libraries(MemLoader MemoryFunctions TimedExecutor ${CMAKE_THREAD_LIBS_INIT})
