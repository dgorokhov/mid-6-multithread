add_subdirectory(queue)
add_subdirectory(thread_pool)

add_library(task_dispatcher
    task_dispatcher.cpp
)

target_link_libraries(task_dispatcher
    PUBLIC
        thread_pool
        queue
)

target_include_directories(task_dispatcher PUBLIC ${CMAKE_SOURCE_DIR}/include)

# --- ПЕРЕНЕСЕНО СЮДА ---
# Создаем исполняемый файл из main.cpp, который теперь лежит рядом
set(target dispatcher)

add_executable(${target} main.cpp)

# Подключаем заголовочные файлы
target_include_directories(${target} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Линкуем библиотеку к исполняемому файлу
target_link_libraries(${target} 
    PRIVATE
        task_dispatcher
)