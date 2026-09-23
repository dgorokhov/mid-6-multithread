#pragma once
#include "queue/queue.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "queue/bounded_queue.hpp"

namespace dispatcher::queue {

class BoundedQueue : public IQueue {
private:
    const int capacity_;
    std::queue<std::function<void()>> queue_;
    
    mutable std::mutex mutex_;
    std::condition_variable cv_push_; // Для ожидания свободного места в push
    std::atomic<bool> is_shutdown_{false};

public:
    explicit BoundedQueue(int capacity);

    void push(std::function<void()> task) override;

    std::optional<std::function<void()>> try_pop() override;

    ~BoundedQueue() override;
};

}  // namespace dispatcher::queue
