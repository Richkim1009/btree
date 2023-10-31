# Main project information
cmake_minimum_required(VERSION 3.14)
project(btree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the main executable
add_executable(btree main.cpp)

# Fetch Google Test content
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Enable CMake's testing features
enable_testing()

# Create an executable file for testing
add_executable(
  first_test
  tests/first_test.cc
)

# Link the GTest::gtest_main library to the test executable
target_link_libraries(
  first_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(first_test)
