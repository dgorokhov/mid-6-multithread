#include "queue/bounded_queue.hpp"

namespace dispatcher::queue {

BoundedQueue::BoundedQueue(int capacity) 
    : capacity_(capacity) {}

void BoundedQueue::push(std::function<void()> task) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Ждем, пока в очереди появится место ИЛИ пока не начнется выключение
    cv_push_.wait(lock, [this]() { 
        return queue_.size() < static_cast<size_t>(capacity_) || is_shutdown_.load(std::memory_order_acquire); 
    });

    // Если система завершает работу, новые задачи не добавляем
    if (is_shutdown_.load(std::memory_order_acquire)) {
        return;
    }

    queue_.push(std::move(task));
}

std::optional<std::function<void()>> BoundedQueue::try_pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (queue_.empty()) {
        return std::nullopt;
    }

    auto task = std::move(queue_.front());
    queue_.pop();

    // Освободилось место — будим один из потоков, застрявших в push
    cv_push_.notify_one();
    
    return task;
}

BoundedQueue::~BoundedQueue() {
    is_shutdown_.store(true, std::memory_order_release);
    // Разблокируем все потоки, которые могли ожидать в push
    cv_push_.notify_all();
}

}  // namespace dispatcher::queue
