#include "thread_pool/thread_pool.hpp"
#include <iostream>

namespace dispatcher::thread_pool {

ThreadPool::ThreadPool(std::shared_ptr<queue::PriorityQueue> queue, size_t threads_count) : queue_(std::move(queue)) {
    workers_.reserve(threads_count);
    for (size_t i = 0; i < threads_count; ++i) {
        workers_.emplace_back(&ThreadPool::worker_routine, this);
    }
}

ThreadPool::~ThreadPool() {
    // Согласно ТЗ, после вызова деструктора потоки исполняют задачи,
    // пока из приоритетной очереди не вернётся std::nullopt.
    for (auto &worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::worker_routine() {
    while (true) {
        auto task_opt = queue_->pop();
        if (!task_opt.has_value()) {
            break;  // Очередь закрыта и пуста, поток завершает работу
        }

        // Выполнение задачи с перехватом исключений (требование ТЗ)
        try {
            if (task_opt.value()) {
                task_opt.value()();
            }
        } catch (const std::exception &e) {
            std::cerr << "[ThreadPool Worker Error]: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "[ThreadPool Worker Error]: Unknown exception caught." << std::endl;
        }
    }
}

}  // namespace dispatcher::thread_pool
