#pragma once
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"
#include "types.hpp"

#include <atomic>
#include <condition_variable>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace dispatcher::queue {

class PriorityQueue {
private:
    
    std::map<TaskPriority, std::unique_ptr<IQueue>, std::greater<TaskPriority>> queues_;

    mutable std::mutex mutex_;
    std::condition_variable cv_pop_;  // Для блокировки потоков в методе pop()
    std::atomic<bool> is_shutdown_{false};

public:
    // Конструктор принимает конфигурацию соответствия приоритета и настроек очереди
    explicit PriorityQueue(const std::map<TaskPriority, QueueOptions> &config);

    void push(TaskPriority priority, std::function<void()> task);

    // локирует поток на pop, пока не будет задача\до вызова shutdown.
    // После shutdown возвращает оставшиеся задачи, когда они закончатся -nullopt.
    std::optional<std::function<void()>> pop();

    void shutdown();

    ~PriorityQueue();
};

}  // namespace dispatcher::queue
