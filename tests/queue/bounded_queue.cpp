#include <gtest/gtest.h>
#include "queue/bounded_queue.hpp"
#include <atomic>
#include <thread>
#include <chrono>

namespace dispatcher::queue::tests {

TEST(BoundedQueueTest, BasicPushPop) {
    BoundedQueue q(2);
    std::atomic<bool> executed{false};
    q.push([&executed]() { executed.store(true, std::memory_order_release); });
    
    auto task_opt = q.try_pop();
    ASSERT_TRUE(task_opt.has_value());
    task_opt.value()();
    EXPECT_TRUE(executed.load(std::memory_order_acquire));
}

TEST(BoundedQueueTest, PopFromEmptyQueue) {
    BoundedQueue q(5);
    auto task_opt = q.try_pop();
    EXPECT_FALSE(task_opt.has_value()); // Должен вернуть std::nullopt
}

}  // namespace dispatcher::queue::tests
