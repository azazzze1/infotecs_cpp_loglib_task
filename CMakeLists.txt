cmake_minimum_required(VERSION 3.14)
project(LoggerLibrary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared library" OFF)

include_directories(${PROJECT_SOURCE_DIR}/logger)

add_subdirectory(logger)
add_subdirectory(server)