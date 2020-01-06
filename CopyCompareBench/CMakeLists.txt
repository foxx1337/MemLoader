project(CopyCompareBench)

add_executable(CopyCompareBench "CopyCompareBench.cpp")

target_link_libraries(CopyCompareBench MemoryFunctions TimedExecutor)
