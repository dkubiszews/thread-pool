include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(TEST_BINARY ${PROJECT_NAME}_test)

add_executable(${TEST_BINARY} pool_test.cpp)

target_link_libraries(${TEST_BINARY} PRIVATE ${PROJECT_NAME} gtest_main)

gtest_discover_tests(${TEST_BINARY})