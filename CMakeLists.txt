cmake_minimum_required(VERSION 3.10)
project(infotecs_cpp_loglib_task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIB "Build shared (dynamic) library" OFF)

set(CMAKE_C_COMPILER gcc)

add_subdirectory(logger)
add_subdirectory(socket_stats_app)
add_subdirectory(logger_test_app)