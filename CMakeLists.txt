cmake_minimum_required(VERSION 3.25)
project(priority_task_dispatcher VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)

# Включаем CTest для тестов
include(CTest)
enable_testing()

# Подключаем исходники (сначала собираем библиотеки, чтобы прилинковать их к main)
add_subdirectory(src)
add_subdirectory(tests)

# Раскомментировано и исправлено для сборки исполняемого файла dispatcher
set(target dispatcher)

add_executable(${target} main.cpp)

# Подключаем заголовочные файлы к исполняемому файлу
target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линкуем наш диспетчер задач к исполняемому файлу
target_link_libraries(${target} 
    PRIVATE
        task_dispatcher
)
