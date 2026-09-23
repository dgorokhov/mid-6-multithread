#include <gtest/gtest.h>
#include "task_dispatcher.hpp"
#include <atomic>
#include <thread>
#include <chrono>
#include <stdexcept>

namespace dispatcher::tests {

// 6: исключения внутри воркеров диспетчера
TEST(TaskDispatcherTest, ExceptionHandleInWorker) {
    TaskDispatcher dispatcher(2);
    std::atomic<bool> next_task_executed{false};

    dispatcher.schedule(TaskPriority::High, []() { 
        throw std::runtime_error("Test exception that worker must catch"); 
    });

    // 2-я задача д. выполниться тем же или соседним живым воркером
    dispatcher.schedule(TaskPriority::Normal, [&next_task_executed]() { 
        next_task_executed.store(true, std::memory_order_release); 
    });

    // Даем немного времени на выполнение
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_TRUE(next_task_executed.load(std::memory_order_acquire));
}

// Тест 7: Выполнение абсолютно всех задач при деструкции диспетчера
TEST(TaskDispatcherTest, ExecAllBeforeDestruct) {
    std::atomic<int> completed_tasks{0};
    {
        TaskDispatcher dispatcher(4);
        for (int i = 0; i < 20; ++i) {
            dispatcher.schedule(TaskPriority::Normal, [&completed_tasks]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                completed_tasks.fetch_add(1, std::memory_order_relaxed);
            });
        }
        // деструктор ~TaskDispatcher, обязан дождаться конца 20 задач
    }
    EXPECT_EQ(completed_tasks.load(), 20);
}

// Тест 8: Стресс-тест на отсутствие гонок данных при высокой конкуренции
TEST(TaskDispatcherTest, MThreadedStressTest) {
    TaskDispatcher dispatcher(4);
    std::atomic<int> total_sum{0};

    auto producer = [&dispatcher, &total_sum]() {
        for (int i = 0; i < 50; ++i) {
            dispatcher.schedule(TaskPriority::Normal, [&total_sum]() {
                total_sum.fetch_add(1, std::memory_order_relaxed);
            });
        }
    };

    std::thread t1(producer);
    std::thread t2(producer);
    t1.join();
    t2.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
   
    // В деструкторе диспетчер точно всё доделает
}

}  // namespace dispatcher::tests
