#include "task_dispatcher.hpp"
#include <cassert>

namespace dispatcher {

TaskDispatcher::TaskDispatcher(size_t thread_count, 
    const std::map<TaskPriority, queue::QueueOptions>& config)
    : queue_(std::make_shared<queue::PriorityQueue>(config))
    , pool_(std::make_unique<thread_pool::ThreadPool>(queue_, thread_count)) {}

void TaskDispatcher::schedule(TaskPriority priority, std::function<void()> task) {
    /*if (queue_) {
        queue_->push(priority, std::move(task));
    }
        */
    assert(queue_);
    queue_->push(priority, std::move(task));          
}

TaskDispatcher::~TaskDispatcher() {
    //if (queue_) {
    //    queue_->shutdown();
    //}
    assert(queue_);
    queue_->shutdown();
}

} // namespace dispatcher
