cmake_minimum_required(VERSION 3.25)
project(priority_task_dispatcher VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Находим зависимости, установленные через Conan (CMakeDeps генератор)
find_package(GTest REQUIRED)

# 2. Включаем CTest для тестов
include(CTest)
enable_testing()

# 3. Подключаем поддиректории
add_subdirectory(src)
add_subdirectory(src/queue)
add_subdirectory(tests)
