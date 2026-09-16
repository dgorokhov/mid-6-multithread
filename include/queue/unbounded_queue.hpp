#pragma once
#include "queue/queue.hpp"
#include <atomic>
#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue {
private:
    std::queue<std::function<void()>> queue_;
    mutable std::mutex mutex_;
    std::atomic<bool> is_shutdown_{false};

public:
    // Исправлено: конструктор теперь без параметров
    UnboundedQueue() = default;

    void push(std::function<void()> task) override;
    std::optional<std::function<void()>> try_pop() override;
    ~UnboundedQueue() override;
};

}  // namespace dispatcher::queue
