enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE logger GTest::GTest GTest::Main pthread)
add_test(NAME tests COMMAND tests)