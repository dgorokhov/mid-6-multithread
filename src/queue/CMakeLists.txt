add_library(queue
    bounded_queue.cpp
    unbounded_queue.cpp
    priority_queue.cpp
)

target_include_directories(queue PUBLIC ${CMAKE_SOURCE_DIR}/include)
