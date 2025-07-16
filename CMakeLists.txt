cmake_minimum_required(VERSION 3.10)
project(infotecs_cpp_loglib_task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_SHARED_LIB "Build shared (dynamic) library" OFF)

file(GLOB LOGGER_SOURCES "logger/*.cpp")

if(BUILD_SHARED_LIB)
    add_library(logger SHARED ${LOGGER_SOURCES})
else()
    add_library(logger STATIC ${LOGGER_SOURCES})
endif()

add_executable(main main.cpp)

target_link_libraries(main PRIVATE logger)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/logger)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(main PRIVATE DEBUG_MODE)
endif()