enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(tests 
    ${TEST_SOURCES} 
    ${PROJECT_SOURCE_DIR}/socket_stats_app/socketStatsCollector.cpp
)

target_include_directories(tests PRIVATE 
    ${PROJECT_SOURCE_DIR}/logger
    ${PROJECT_SOURCE_DIR}/socket_stats_app
)

target_link_libraries(tests PRIVATE logger GTest::GTest GTest::Main pthread)
add_test(NAME tests COMMAND tests)