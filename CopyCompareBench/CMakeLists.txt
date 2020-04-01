project(CopyCompareBench)

add_executable(CopyCompareBench "CopyCompareBench.cpp")

find_package(Threads)
target_link_libraries(CopyCompareBench MemoryFunctions TimedExecutor ${CMAKE_THREAD_LIBS_INIT})
