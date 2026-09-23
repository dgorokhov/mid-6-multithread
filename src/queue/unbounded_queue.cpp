#include "queue/unbounded_queue.hpp"

namespace dispatcher::queue {

void UnboundedQueue::push(std::function<void()> task) {
    if (is_shutdown_.load(std::memory_order_acquire)) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(task));
}

std::optional<std::function<void()>> UnboundedQueue::try_pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (queue_.empty()) {
        return std::nullopt;
    }

    auto task = std::move(queue_.front());
    queue_.pop();
    
    return task;
}

UnboundedQueue::~UnboundedQueue() {
    is_shutdown_.store(true, std::memory_order_release);
}

}  // namespace dispatcher::queue
