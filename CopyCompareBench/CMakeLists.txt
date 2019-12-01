cmake_minimum_required(VERSION 3.15)

project(CopyCompareBench)

include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

add_library(MemoryFunctions OBJECT
    "MemoryFunctions.cpp"
    "MemoryFunctions.h")

add_executable(CopyCompareBench
    $<TARGET_OBJECTS:MemoryFunctions>
    "CopyCompareBench.cpp"
    "TimedExecutor.h")

add_executable(MemoryFunctions_Tests
    $<TARGET_OBJECTS:MemoryFunctions>
    "tests.cpp")

target_link_libraries(MemoryFunctions_Tests PRIVATE GTest::gtest_main)

gtest_discover_tests(MemoryFunctions_Tests)
