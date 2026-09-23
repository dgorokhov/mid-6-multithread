#include "queue/priority_queue.hpp"

namespace dispatcher::queue {


PriorityQueue::PriorityQueue(const std::map<TaskPriority, QueueOptions>& config) {
    for (const auto& [priority, options] : config) {
        if (options.bounded) {
            // Исправлено: извлекаем int из std::optional с помощью .value_or
            queues_[priority] = std::make_unique<BoundedQueue>(options.capacity.value_or(1000));
        } else {
            queues_[priority] = std::make_unique<UnboundedQueue>();
        }
    }
}

void PriorityQueue::push(TaskPriority priority, std::function<void()> task) {
    if (is_shutdown_.load(std::memory_order_acquire)) {
        return;
    }

    auto it = queues_.find(priority);
    if (it != queues_.end()) {
        it->second->push(std::move(task));
        // Оповещаем один из потоков, ожидающих в pop()
        cv_pop_.notify_one();
    }
}


std::optional<std::function<void()>> PriorityQueue::pop() {
   
    std::unique_lock<std::mutex> lock(mutex_);

    while (true) {
        // Проверяем очереди строго по приоритетам (High идет первым из-за std::greater)
        for (auto& [priority, queue] : queues_) {
            if (auto task_opt = queue->try_pop()) {
                return task_opt; // Нашли задачу — сразу возвращаем её
            }
        }
        if (is_shutdown_.load(std::memory_order_acquire)) {
            return std::nullopt;
        }
        cv_pop_.wait(lock);
    }
}

void PriorityQueue::shutdown() {
    //amend
    std::lock_guard<std::mutex> lock(mutex_);
    is_shutdown_.store(true, std::memory_order_release);
    cv_pop_.notify_all();
}

PriorityQueue::~PriorityQueue() {
    shutdown();
}

}  
