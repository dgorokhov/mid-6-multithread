#include <gtest/gtest.h>
#include "queue/unbounded_queue.hpp"
#include <vector>
#include <string>

namespace dispatcher::queue::tests {

// Тест 3: Проверка порядка FIFO в неограниченной очереди
TEST(UnboundedQueueTest, FifoOrder) {
    UnboundedQueue q;
    std::vector<int> results;

    // Пушим задачи, записывающие числа по порядку
    q.push([&results]() { results.push_back(1); });
    q.push([&results]() { results.push_back(2); });
    q.push([&results]() { results.push_back(3); });

    // Извлекаем и выполняем их одну за другой
    for (int i = 0; i < 3; ++i) {
        auto task_opt = q.try_pop();
        ASSERT_TRUE(task_opt.has_value());
        task_opt.value()();
    }

    // Порядок должен быть строго сохранен: 1, 2, 3
    ASSERT_EQ(results.size(), 3);
    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 2);
    EXPECT_EQ(results[2], 3);
}

}  // namespace dispatcher::queue::tests
