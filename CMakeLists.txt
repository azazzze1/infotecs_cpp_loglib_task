cmake_minimum_required(VERSION 3.14)
project(LoggerTest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Только статическая библиотека
option(BUILD_SHARED_LIBS "Build shared library" OFF)

# Добавляем заголовки
include_directories(include)

# Собираем статическую библиотеку из src/
file(GLOB SRC_FILES "logger/*.cpp")
add_library(logger STATIC ${SRC_FILES})

# Собираем тестовое приложение из main.cpp
add_executable(logger_test main.cpp)
target_link_libraries(logger_test PRIVATE logger)
target_include_directories(logger_test PRIVATE include)