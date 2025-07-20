cmake_minimum_required(VERSION 3.10)
project(infotecs_cpp_loglib_task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIB "Build shared (dynamic) library" OFF)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE PATH "Installation directory")

set(BUILD_SHARED_LIB ${BUILD_SHARED_LIB} CACHE INTERNAL "Type of library to build")

# Добавление подпроектов
add_subdirectory(logger)
add_subdirectory(logger_test_app)
add_subdirectory(socket_stats_app)
add_subdirectory(tests)