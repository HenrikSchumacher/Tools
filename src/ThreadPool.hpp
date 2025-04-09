#pragma once

// Implementation taken from
// https://github.com/bshoshany/thread-pool


// TODO: Thread affinity
// See https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/
 
#include <atomic>             // std::atomic
#include <condition_variable> // std::condition_variable
#include <exception>          // std::current_exception
#include <functional>         // std::bind, std::function, std::invoke
#include <future>             // std::future, std::promise
#include <memory>             // std::make_shared, std::make_unique, std::shared_ptr, std::unique_ptr
#include <mutex>              // std::mutex, std::scoped_lock, std::unique_lock
#include <queue>              // std::queue
#include <thread>             // std::thread
#include <vector>             // std::vector
#include <array>              // std::array
#include <type_traits>        // std::common_type_t, std::decay_t, std::invoke_result_t, std::is_void_v
#include <utility>            // std::forward, std::move, std::swap

namespace Tools
{
 
    // This class is basically BS_thread_pool from https://github.com/bshoshany/thread-pool with a couple of functionalities removed and others added.
    
    class ThreadPool
    {
    public:
        
        using Int = Size_T;
        
        ThreadPool() = default;
        
        ~ThreadPool()
        {
            WaitForTasks();
            DestroyThreads();
        }
        
        Int ThreadCount() const
        {
            return static_cast<Int>(threads.size());
        }
        
        void SetThreadCount( const Int thread_count_ )
        {
            WaitForTasks();
            DestroyThreads();
//            TOOLS_DUMP(std::thread::hardware_concurrency());
            
            Int thread_count = (thread_count_ > Int(0)) ? thread_count_ :  Int(1);
//            :   Max(
//                         Int(1),
//                         static_cast<Int>(std::thread::hardware_concurrency())
//                );
            
            threads = std::vector<std::thread>       ( thread_count );
            
            running = true;
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                threads[thread] = std::thread(&ThreadPool::Worker, this);
            }
        }
        
        void RequireThreads( const Int thread_count )
        {
            if( thread_count > ThreadCount() )
            {
//                print("RequireThreads");
//                valprint("old number of threads",ThreadCount());
                SetThreadCount( thread_count );
//                valprint("new number of threads",ThreadCount());
            }
        }
        
        
        
        // Executes the function `fun` of the form `[]( const Int thread ) -> void {...}` parallelized over `thread_count` threads.
        template<typename F, typename I>
        void Do( F && fun, const I thread_count )
        {
            if( thread_count <= static_cast<I>(1) )
            {
                std::invoke( fun, static_cast<I>(0) );
            }
            else
            {
                RequireThreads( static_cast<Int>(thread_count) );
                
                std::vector<std::future<void>> futures ( thread_count );
                
                for( I thread = 0; thread < thread_count; ++thread )
                {
                    futures[thread] = Submit( std::forward<F>(fun), thread );
                }
                
                for( I thread = 0; thread < thread_count; ++thread )
                {
                    futures[thread].get();
                }
            }
        }
        
        // Executes the function `fun` of the form `[]( const Int thread ) -> void {...}` parallelized over `thread_count` threads.
        // Afterwards, reduces with the function `reducer` of the form `[]( const Int thread, const S & value, T & result ) {...}` .
        template<typename F, typename R, typename I, typename T, typename S = std::invoke_result_t<std::decay_t<F>, Int>
        >
        T DoReduce( F && fun, R && reducer, const T & init_value, const I thread_count )
        {
            
            T result (init_value);
            
            if( thread_count <= static_cast<I>(1) )
            {
                std::invoke(
                    reducer,
                    static_cast<I>(0),
                    std::invoke( fun, static_cast<I>(0) ),
                    result
                );
            }
            else
            {
                RequireThreads( static_cast<Int>(thread_count) );
                
                std::vector<std::future<S>> futures ( thread_count );
                
                for( I thread = 0; thread < thread_count; ++thread )
                {
                    // Shall we make copies fo fun in case it has state?
                    // Or shall we rather std::forward it?
                    futures[thread] = Submit( fun, thread );
                }
                
                for( I thread = 0; thread < thread_count; ++thread )
                {
                    std::invoke( reducer, thread, futures[thread].get(), result );
                }
            }

            return result;
        }
        
        
//        // Executes function `fun` of the form []( const I i ) -> void {} in parallel for argument i ranging from begin to end.
//        template<typename F, typename R, typename I>
//        void Map(
//            F && fun,
//            const I begin,
//            const I end,
//            const Int thread_count
//        )
//        {
//            return Do(
//               [fun,begin,end,thread_count]( const Int thread ) -> void
//               {
//                   const I i_begin = begin + JobPointer( end - begin, thread_count, thread   );
//                   const I i_end   = begin + JobPointer( end - begin, thread_count, thread+1 );
//
//                   for( I i = i_begin; i < i_end; ++i )
//                   {
//                       fun( i );
//                   }
//               },
//               thread_count
//            );
//        }
        
//        // Same as above but with precomputed JobPointers.
//        template<typename F, typename R, typename I>
//        void Map( F && fun, const JobPointers<I> & job_ptr )
//        {
//            return Do(
//               [fun,&job_ptr]( const Int thread ) -> void
//               {
//                   const I i_begin = job_ptr[thread  ];
//                   const I i_end   = job_ptr[thread+1];
//
//                   for( I i = i_begin; i < i_end; ++i )
//                   {
//                       fun( i );
//                   }
//               },
//               job_ptr.ThreadCount()
//            );
//        }
        
//        // Executes function `fun` of the form `[]( const I i ) -> void {}` in parallel for argument i ranging from begin to end.
//        // Then recursively runs reduction function `reducer` of the form `[]( cost Int thread, const S &, T & result ){...}`.
//        template<typename F, typename R, typename I, typename T>
//        T MapReduce(
//            F && fun,
//            R && reducer,
//            const T & init,
//            const I begin,
//            const I end,
//            const Int thread_count
//        )
//        {
//            return DoReduce(
//               [fun,begin,end,thread_count]( const Int thread ) -> void
//               {
//                   const I i_begin = begin + JobPointer( end - begin, thread_count, thread   );
//                   const I i_end   = begin + JobPointer( end - begin, thread_count, thread+1 );
//
//                   for( I i = i_begin; i < i_end; ++i )
//                   {
//                       fun( i );
//                   }
//               },
//               std::forward<R>(reducer),
//               init,
//               thread_count
//            );
//        }
//        
//        // Same as above but with precomputed JobPointers.
//        template<typename F, typename R, typename I, typename T>
//        T MapReduce( F && fun, R && reducer, const T & init, const JobPointers<I> & job_ptr
//        )
//        {
//            return DoReduce(
//                [fun,&job_ptr]( const Int thread ) -> void
//                {
//                    const I i_begin = job_ptr[thread  ];
//                    const I i_end   = job_ptr[thread+1];
//
//                    for( I i = i_begin; i < i_end; ++i )
//                    {
//                        fun( i );
//                    }
//                },
//                std::forward<R>(reducer),
//                init,
//                job_ptr.ThreadCount()
//            );
//        }
        
        
    protected:
                             
         /**
          * @brief Push a function with zero or more arguments, but no return value, into the task queue. Does not return a future, so the user must use wait_for_tasks() or some other method to ensure that the task finishes executing, otherwise bad things will happen.
          *
          * @tparam F The type of the function.
          * @tparam A The types of the arguments.
          * @param task The function to push.
          * @param args The zero or more arguments to pass to the function. Note that if the task is a class member function, the first argument must be a pointer to the object, i.e. &object (or this), followed by the actual arguments.
                          */
        template <typename F, typename... A>
        void PushTask( F&& task, A&&... args )
        {
            std::function<void()> fun = std::bind(std::forward<F>(task), std::forward<A>(args)...);
            {
                const std::scoped_lock tasks_lock(tasks_mutex);
                tasks.push(fun);
            }
            ++tasks_total;
            task_available_cv.notify_one();
        }
                         
        /**
         * @brief Submit a function with zero or more arguments into the task queue. If the function has a return value, get a future for the eventual returned value. If the function has no return value, get an std::future<void> which can be used to wait until the task finishes.
         *
         * @tparam F The type of the function.
         * @tparam A The types of the zero or more arguments to pass to the function.
         * @tparam R The return type of the function (can be void).
         * @param task The function to submit.
         * @param args The zero or more arguments to pass to the function. Note that if the task is a class member function, the first argument must be a pointer to the object, i.e. &object (or this), followed by the actual arguments.
         * @return A future to be used later to wait for the function to finish executing and/or obtain its returned value if it has one.
         */
        template <typename F, typename... A, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<A>...>>
        std::future<R> Submit(F&& task, A&&... args)
        {
            std::function<R()> fun = std::bind( std::forward<F>(task), std::forward<A>(args)... );
            
            std::shared_ptr<std::promise<R>> task_promise = std::make_shared<std::promise<R>>();
            
            PushTask(
                 [fun, task_promise]
                 {
                     try
                     {
                         if constexpr( std::is_void_v<R> )
                         {
                             std::invoke(fun);
                             task_promise->set_value();
                         }
                         else
                         {
                             task_promise->set_value(std::invoke(fun));
                         }
                     }
                     catch (...)
                     {
                         try
                         {
                             task_promise->set_exception( std::current_exception() );
                         }
                         catch (...)
                         {
                         }
                     }
                 }
            );
            
            return task_promise->get_future();
        }
        
        /**
         * @brief Wait for tasks to be completed. Normally, this function waits for all tasks, both those that are currently running in the threads and those that are still waiting in the queue. Note: To wait for just one specific task, use Submit() instead, and call the wait() member function of the generated future.
         */
        void WaitForTasks()
        {
            waiting = true;
            std::unique_lock<std::mutex> tasks_lock(tasks_mutex);
            task_done_cv.wait(tasks_lock, [this] { return (tasks_total == 0); });
            waiting = false;
        }
        
    private:
        // ========================
        // Private member functions
        // ========================
        
        /**
         * @brief Create the threads in the pool and assign a worker to each thread.
         */
        void CreateThreads()
        {
            running = true;
            
            for( Int thread = 0; thread < ThreadCount(); ++thread )
            {
                threads[thread] = std::thread(&ThreadPool::Worker, this);
            }
        }
        
        /**
         * @brief Destroy the threads in the pool.
         */
        void DestroyThreads()
        {
            running = false;
            task_available_cv.notify_all();
            
            for( Int thread = 0; thread < ThreadCount(); ++thread )
            {
                threads[thread].join();
            }
        }
        
        /**
         * @brief A worker function to be assigned to each thread in the pool. Waits until it is notified by PushTask() that a task is available, and then retrieves the task from the queue and executes it. Once the task finishes, the worker notifies WaitForTasks() in case it is waiting.
         */
        void Worker()
        {
            while( running )
            {
                std::function<void()> task;
                std::unique_lock<std::mutex> tasks_lock( tasks_mutex );
                task_available_cv.wait(tasks_lock, [this] { return !tasks.empty() || !running; });
                if( running )
                {
                    task = std::move(tasks.front());
                    tasks.pop();
                    tasks_lock.unlock();
                    task();
                    tasks_lock.lock();
                    --tasks_total;
                    if( waiting )
                    {
                        task_done_cv.notify_one();
                    }
                }
            }
        }
        
        // ============
        // Private data
        // ============
        
        /**
         * @brief An atomic variable indicating to the workers to keep running. When set to false, the workers permanently stop working.
         */
        std::atomic<bool> running = false;
        
        /**
         * @brief A condition variable used to notify Worker() that a new task has become available.
         */
        std::condition_variable task_available_cv = {};
        
        /**
         * @brief A condition variable used to notify wait_for_tasks() that a tasks is done.
         */
        std::condition_variable task_done_cv = {};
        
        /**
         * @brief A queue of tasks to be executed by the threads.
         */
        std::queue<std::function<void()>> tasks = {};
        
        /**
         * @brief An atomic variable to keep track of the total number of unfinished tasks - either still in the queue, or running in a thread.
         */
        std::atomic<Size_T> tasks_total = 0;
        
        /**
         * @brief A mutex to synchronize access to the task queue by different threads.
         */
        mutable std::mutex tasks_mutex = {};
        
        /**
         * @brief A vector storing the threads.
         */
        
        std::vector<std::thread> threads;
        
        /**
         * @brief An atomic variable indicating that WaitForTasks() is active and expects to be notified whenever a task is done.
         */
        std::atomic<bool> waiting = false;
                         
                         
    }; // class ThreadPool

    inline ThreadPool thread_pool;
    
} // namespace Tools
