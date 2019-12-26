project(MemoryFunctions)

include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

add_library(MemoryFunctions
    "MemoryFunctions.cpp"
    "MemoryFunctions.h")

add_executable(MemoryFunctions_Tests
    "tests.cpp"
    "MemoryFunctions.h")

target_link_libraries(MemoryFunctions_Tests PRIVATE GTest::gtest_main MemoryFunctions)

gtest_discover_tests(MemoryFunctions_Tests)
