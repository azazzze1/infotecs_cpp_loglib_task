file(GLOB SRC_FILES "*.cpp")

add_library(logger_static STATIC ${SRC_FILES})
add_library(logger_shared SHARED ${SRC_FILES})

set_target_properties(logger_shared PROPERTIES OUTPUT_NAME "logger")
set_target_properties(logger_static PROPERTIES OUTPUT_NAME "logger")

target_include_directories(logger_shared PUBLIC ${PROJECT_SOURCE_DIR}/logger)
target_include_directories(logger_static PUBLIC ${PROJECT_SOURCE_DIR}/logger)

if(BUILD_SHARED_LIBS)
    add_library(logger INTERFACE)
    target_link_libraries(logger INTERFACE logger_shared)
else()
    add_library(logger INTERFACE)
    target_link_libraries(logger INTERFACE logger_static)
endif()

add_executable(logger_test ../main.cpp)
target_link_libraries(logger_test PRIVATE logger)