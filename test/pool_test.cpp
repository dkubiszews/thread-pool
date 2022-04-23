#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

#include <dkubiszewski/threading/pool.hpp>

TEST(Pool, run_empty_pool)
{
  dkubiszewski::threading::Pool pool{4};

  pool.Run();
}

TEST(Pool, schedule_one_thread)
{
  std::atomic_bool thread_executed{false};
  dkubiszewski::threading::Pool pool{1};

  pool.Enqueue([&thread_executed]()
               { thread_executed.store(true); });
  pool.Run();

  EXPECT_TRUE(thread_executed.load());
}

auto provide_function_adding_to_atomic(std::atomic_uint &atomic_value, unsigned int value)
{
  return [&atomic_value, value]()
  {
                 using namespace std::chrono_literals;
                 atomic_value.fetch_add(value);
                 std::this_thread::sleep_for(100ms); };
}

TEST(Pool, schedule_multiple_threads)
{
  dkubiszewski::threading::Pool pool{2};
  std::atomic_uint value{0};

  pool.Enqueue(provide_function_adding_to_atomic(value, 1));
  pool.Enqueue(provide_function_adding_to_atomic(value, 20));
  pool.Enqueue(provide_function_adding_to_atomic(value, 300));
  pool.Enqueue(provide_function_adding_to_atomic(value, 4000));

  pool.Run();

  EXPECT_EQ(4321, value.load());
}

auto provide_function_adding_determine_max_threads_number(std::mutex &mutex, unsigned int &max_threads_number, unsigned int &current_threads_number)
{
  return [&mutex, &max_threads_number, &current_threads_number]()
  {
    using namespace std::chrono_literals;
    {
      std::unique_lock<std::mutex> lk(mutex);
      current_threads_number++;
      max_threads_number = std::max(max_threads_number, current_threads_number);
    }
    std::this_thread::sleep_for(100ms);
    {
      std::unique_lock<std::mutex> lk(mutex);
      current_threads_number--;
    }
  };
}

TEST(Pool, started_threads_are_limited)
{
  constexpr std::size_t max_threads = 4;
  dkubiszewski::threading::Pool pool{max_threads};
  unsigned int max_threads_number{0};
  unsigned int current_threads_number{0};
  std::mutex mutex;

  for (std::size_t i = 0; i < 20; ++i)
  {
    pool.Enqueue(provide_function_adding_determine_max_threads_number(mutex, max_threads_number, current_threads_number));
  }

  pool.Run();

  EXPECT_EQ(max_threads, max_threads_number);
}