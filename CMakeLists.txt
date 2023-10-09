cmake_minimum_required(VERSION 3.14)
project(btree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# CMake의 테스팅 기능을 활성화합니다. 이 명령을 사용하면 ctest와 같은 도구를 사용하여 테스트를 실행할 수 있게 됩니다.
enable_testing() 

# hello_test라는 이름의 실행 파일을 생성합니다. 이 실행 파일의 소스 코드로 tests/hello_test.cc를 사용합니다
add_executable(
  first_test
  tests/first_test.cc
)

# hello_test 실행 파일에 GTest::gtest_main 라이브러리를 링크합니다. GTest::gtest_main은 Google Test 프레임워크의 메인 함수를 제공하는 라이브러리입니다. 
# 이를 통해 개별 테스트 케이스를 작성할 때 별도의 main 함수를 제공할 필요가 없습니다
target_link_libraries(
  first_test
  GTest::gtest_main
)

include(GoogleTest)
# gtest_discover_tests 함수는 주어진 대상 (hello_test)에 대한 테스트를 자동으로 발견하고 등록합니다. 
# 이렇게 하면 ctest를 사용하여 모든 테스트를 실행할 때, hello_test 실행 파일 내의 모든 Google Test 테스트 케이스들이 자동으로 실행됩니다.
gtest_discover_tests(first_test)