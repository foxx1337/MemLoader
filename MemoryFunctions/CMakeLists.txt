project(MemoryFunctions)

include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

add_library(MemoryFunctions
    "MemoryFunctions.cpp"
    "MemoryFunctions.h")
target_include_directories(MemoryFunctions PUBLIC "${MemoryFunctions_SOURCE_DIR}")

add_executable(MemoryFunctions_Tests
    "tests.cpp"
    "MemoryFunctions.h")

target_link_libraries(MemoryFunctions_Tests PRIVATE GTest::gtest_main MemoryFunctions)

gtest_discover_tests(MemoryFunctions_Tests)
