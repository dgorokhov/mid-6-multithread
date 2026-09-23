#pragma once

#include <memory>
#include <map>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

namespace dispatcher {

class TaskDispatcher {
private:
    std::shared_ptr<queue::PriorityQueue> queue_;
    std::unique_ptr<thread_pool::ThreadPool> pool_;

public:
    explicit TaskDispatcher(
        size_t thread_count,
        const std::map<TaskPriority, queue::QueueOptions>& config = {
            {TaskPriority::High, queue::QueueOptions{true, std::optional<int>{1000}}},
            {TaskPriority::Normal, queue::QueueOptions{false, std::nullopt}}
        }
    );

    void schedule(TaskPriority priority, std::function<void()> task);
    
    ~TaskDispatcher();

    TaskDispatcher(const TaskDispatcher&) = delete;
    TaskDispatcher& operator=(const TaskDispatcher&) = delete;
};

}  // namespace dispatcher
