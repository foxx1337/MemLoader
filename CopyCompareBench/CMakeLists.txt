project(CopyCompareBench)

include_directories("${MemoryFunctions_SOURCE_DIR}")

add_executable(CopyCompareBench
    "CopyCompareBench.cpp"
    "../src/TimedExecutor.h")

target_link_libraries(CopyCompareBench MemoryFunctions)
