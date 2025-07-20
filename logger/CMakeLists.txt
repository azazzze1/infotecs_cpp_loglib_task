file(GLOB LOGGER_HEADERS "*.hpp")
file(GLOB LOGGER_SOURCES "*.cpp")

if(BUILD_SHARED_LIB)
    add_library(logger SHARED ${LOGGER_SOURCES})
else()
    add_library(logger STATIC ${LOGGER_SOURCES})
endif()

target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

export(TARGETS logger FILE logger-config.cmake)

install(TARGETS logger DESTINATION lib)
install(FILES ${LOGGER_HEADERS} DESTINATION include/logger)