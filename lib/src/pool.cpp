#include <dkubiszewski/threading/pool.hpp>

#include <thread>
#include <mutex>

namespace dkubiszewski
{
  namespace threading
  {

    Pool::Pool(std::size_t threads_number) : _max_threads{threads_number} {}
    void Pool::Run()
    {
      std::vector<std::thread> threads;
      std::mutex threads_mutex;
      for (std::size_t i = 0; i < _max_threads; ++i)
      {
        threads.push_back(std::thread{[&threads_mutex, this]()
                                      {
                                        while (true)
                                        {
                                          threads_mutex.lock();

                                          if (_functions_queue.empty())
                                          {
                                            threads_mutex.unlock();
                                            return;
                                          }

                                          auto function_to_execute = _functions_queue.front();
                                          _functions_queue.pop();

                                          threads_mutex.unlock();
                                          function_to_execute();
                                        }
                                      }});
      }

      for (auto &thread : threads)
      {
        thread.join();
      }
    }

    // TODO: implement move enabled version of this function.
    // TODO: implement thread safe version.
    void Pool::Enqueue(ThreadFunctionType thread_function)
    {
      _functions_queue.push(std::move(thread_function));
    }

  } // namespace threading
} // namespace mylib