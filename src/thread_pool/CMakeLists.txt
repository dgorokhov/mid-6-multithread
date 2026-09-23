add_library(thread_pool
    thread_pool.cpp
)

target_include_directories(thread_pool PUBLIC ${CMAKE_SOURCE_DIR}/include)
