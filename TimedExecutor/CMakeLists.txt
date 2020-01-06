project(TimedExecutor)

add_library(TimedExecutor INTERFACE)
target_include_directories(TimedExecutor INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(TimedExecutor INTERFACE "TimedExecutor.h")
