#ifndef _DKUBISZEWSKI_THREADING_THREADPOOL_HPP_
#define _DKUBISZEWSKI_THREADING_THREADPOOL_HPP_

#include <functional>
#include <queue>

namespace dkubiszewski
{
  namespace threading
  {

    /**
     * @brief Thread pool class.
     */
    class Pool
    {
    public:
      using ThreadFunctionType = std::function<void()>;
      /**
       * @param threads_number Number of threads in the pool.
       */
      Pool(std::size_t threads_number);

      /**
       * @brief Run the thread pool.
       * @note This function blocks until all enqueued functions are executed.
       */
      void Run();

      /**
       * @brief Enqueue function to be scheduled on the thread pool.
       *
       * @param thread_function Function to be scheduled on the thread pool.
       */
      void Enqueue(ThreadFunctionType thread_function);

    private:
      const std::size_t _max_threads;
      std::queue<ThreadFunctionType> _functions_queue;
    };

  } // namespace threading
} // namespace mylib

#endif // _DKUBISZEWSKI_THREADING_THREADPOOL_HPP_