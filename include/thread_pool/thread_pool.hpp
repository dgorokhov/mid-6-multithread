#pragma once
#include "queue/priority_queue.hpp"
#include <vector>
#include <thread>
#include <memory>

namespace dispatcher::thread_pool {

class ThreadPool {
private:
    std::shared_ptr<queue::PriorityQueue> queue_;
    std::vector<std::thread> workers_;

    // Приватный метод, который запускает каждый рабочий поток
    void worker_routine();

public:
    ThreadPool(std::shared_ptr<queue::PriorityQueue> queue, size_t threads_count);
    ~ThreadPool();

    // Запрещаем копирование и присваивание для безопасности работы с потоками
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
};

} // namespace dispatcher::thread_pool
