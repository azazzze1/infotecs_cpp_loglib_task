# Сканируем все .cpp файлы в папке src
file(GLOB SRC_FILES "*.cpp")

# Создаём статическую библиотеку
add_library(logger STATIC ${SRC_FILES})

# Экспортируем заголовочные файлы
target_include_directories(logger PUBLIC ${PROJECT_SOURCE_DIR}/include)