add_executable(logger_test main.cpp appController.cpp threadLogQueue.cpp)

target_link_libraries(logger_test PRIVATE logger)

target_include_directories(logger_test PRIVATE ${PROJECT_SOURCE_DIR}/logger)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(logger_test PRIVATE DEBUG_MODE)
endif()