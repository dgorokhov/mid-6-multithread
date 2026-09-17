#include <gtest/gtest.h>
#include "queue/priority_queue.hpp"
#include <vector>
#include <map>

namespace dispatcher::queue::tests {

TEST(PriorityQueueTest, HighPriorityFirst) {
    std::map<TaskPriority, QueueOptions> config = {
        {TaskPriority::High, QueueOptions{false, std::nullopt}},
        {TaskPriority::Normal, QueueOptions{false, std::nullopt}}
    };
    PriorityQueue pq(config);
    std::vector<int> execution_order;

    // Шаг 1: Добавляем задачу с НОРМАЛЬНЫМ приоритетом
    pq.push(TaskPriority::Normal, [&execution_order]() { execution_order.push_back(0); });
    
    pq.push(TaskPriority::High, [&execution_order]() { execution_order.push_back(1); });

    // Шаг 3: Извлекаем первую задачу. Благодаря правильной сортировке map,
    // метод pop() обязан проигнорировать Normal и вернуть High.
    auto t1 = pq.pop();
    ASSERT_TRUE(t1.has_value());
    t1.value()(); // Выполнит добавление '1'

    // Шаг 4: Извлекаем вторую задачу (оставшийся Normal)
    auto t2 = pq.pop();
    ASSERT_TRUE(t2.has_value());
    t2.value()(); // Выполнит добавление '0'

    // Проверяем порядок выполнения
    ASSERT_EQ(execution_order.size(), 2);
    EXPECT_EQ(execution_order[0], 1); // Первым выполнился High
    EXPECT_EQ(execution_order[1], 0); // Вторым выполнился Normal
}

// Тест 5: Работа pop() и сохранение остатков после shutdown()
TEST(PriorityQueueTest, DrainAfterShutdown) {
    std::map<TaskPriority, QueueOptions> config = {
        {TaskPriority::Normal, QueueOptions{false, std::nullopt}}
    };
    PriorityQueue pq(config);
    int counter = 0;

    pq.push(TaskPriority::Normal, [&counter]() { counter += 1; });
    
    // Выключаем очередь
    pq.shutdown();

    // Задача всё ещё должна быть доступна для извлечения
    auto t1 = pq.pop();
    ASSERT_TRUE(t1.has_value());
    t1.value()();
    EXPECT_EQ(counter, 1);

    // Следующий вызов на пустой очереди после shutdown должен вернуть nullopt
    auto t2 = pq.pop();
    EXPECT_FALSE(t2.has_value());
}

}  // namespace dispatcher::queue::tests
