#pragma once

// Implementation taken from
// https://github.com/bshoshany/thread-pool


// TODO: Thread affinity
// See https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/


// TODO: Use std::this_thread::get_id()
 
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
        
        using Int = std::size_t;
        
        //        ThreadPool()
        //        {
        //            thread_pool_print("ThreadPool() default constructor");
        //            SetThreadCount(1);
        //        }
        
        ThreadPool() = default;
        
        explicit ThreadPool( const Int thread_count_ )
        {
            SetThreadCount(thread_count_);
        }
        
        ~ThreadPool()
        {
            WaitForTasks();
            DestroyThreads();
        }
        
        Int ThreadCount() const
        {
            return thread_count;
        }
        
        void SetThreadCount( const Int thread_count_ )
        {
            WaitForTasks();
            DestroyThreads();
            dump(std::thread::hardware_concurrency());
            
            thread_count = (thread_count_ > static_cast<Int>(0))
            ?   thread_count_
            :   std::max(
                         static_cast<Int>(1),
                         static_cast<Int>(std::thread::hardware_concurrency())
                );
            
            dump(thread_count);
            
            threads = std::vector<std::thread>       ( thread_count );
            
            futures = std::vector<std::future<void>> ( thread_count );
            
            running = true;
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                threads[thread] = std::thread(&ThreadPool::Worker, this);
            }
        }
        
        void RequireThreads( const Int thread_count_ )
        {
            //            thread_pool_valprint("thread_count_",thread_count_);
            if( thread_count_ > thread_count )
            {
                print("RequireThreads");
                valprint("old number of threads",thread_count);
                SetThreadCount( thread_count_ );
                valprint("new number of threads",thread_count);
            }
        }
        
        template<typename F>
        void Execute( F && fun, const Int thread_count_ )
        {
            RequireThreads( thread_count_ );
            
            for( Int thread = 0; thread < thread_count_; ++thread )
            {
                futures[thread] = Submit( std::forward<F>(fun), thread );
            }
            
            for( Int thread = 0; thread < thread_count_; ++thread )
            {
                futures[thread].get();
            }
        }
        
        template<typename F>
        void QueuedExecute( F && fun, const Int job_count_, const Int thread_count_ )
        {
            print("QueuedExecute");
            
            RequireThreads( thread_count_ );
            
            std::vector<std::future<void>> futures_ (job_count_);
            
            for( Int job = 0; job < job_count_; ++job )
            {
                futures_[job] = Submit( std::forward<F>(fun), job/thread_count_, job );
            }
            
            for( Int job = 0; job < job_count_; ++job )
            {
                futures_[job].get();
            }
        }
        
        template<typename F, typename I>
        void BalancedMap( F && fun, const JobPointers<I> & job_ptr )
        {
            const Int thread_count_ = job_ptr.ThreadCount();
            
            RequireThreads( thread_count_ );
            
//            print("BalancedMap");
            
            for( Int thread = 0; thread < thread_count_; ++thread )
            {
                futures[thread] = Submit(
                    std::forward<F>(fun),
                    thread, job_ptr[thread], job_ptr[thread+1]
                );
            }
            
            for( Int thread = 0; thread < thread_count_; ++thread )
            {
                futures[thread].get();
            }
        }
        
        template<typename F, typename I, typename T = std::invoke_result_t<std::decay_t<F>, I, I, I>>
        T BalancedSum( F && fun, const JobPointers<I> & job_ptr )
        {
            const Int thread_count_ = job_ptr.ThreadCount();
            
            RequireThreads( thread_count_ );
            
            std::vector<std::future<T>> futures_ (thread_count_);
            
            for( Int thread = 0; thread < thread_count_; ++thread )
            {
                futures_[thread] = Submit(
                    std::forward<F>(fun),
                    thread, job_ptr[thread], job_ptr[thread+1]
                );
            }
            
            T sum = static_cast<T>(0);
            
            for( Int thread = 0; thread < thread_count_; ++thread )
            {
                sum += futures_[thread].get();
            }
            
            return sum;
        }
        
        
        template<typename F, typename I0, typename I1, typename I2, typename I = std::common_type_t<I0,I1,I2>>
        void Map( F && fun, const I0 begin, const I1 end, const I2 thread_count_ )
        {
//            print("Map");
            Blocks<I> blks (begin, end, thread_count_ ? thread_count_ : static_cast<I>(thread_count) );
            
            RequireThreads( blks.BlockCount() );
            
            if( blks.TotalSize() > 0 )
            {
                for( I thread = 0; thread < blks.BlockCount(); ++thread )
                {
                    futures[thread] = Submit(
                         std::forward<F>(fun),
                         thread, blks.begin(thread), blks.end(thread)
                     );
                }
            }
            
            for( I thread = 0; thread < blks.BlockCount(); ++thread )
            {
                futures[thread].get();
            }
        }
        
        template<typename F, typename I0, typename I1, typename I = std::common_type_t<I0,I1>>
        void Map( F && fun, const I0 begin, const I1 end )
        {
            Map( std::forward<F>(fun), begin, end, thread_count );
        }
        
        template<typename F, typename I, typename T = std::invoke_result_t<std::decay_t<F>, I, I, I>>
        T Sum( F && fun, const I begin, const I end, const I thread_count_ = 0 )
        {
//            print("Sum");
            Blocks<Int> blks (begin, end, thread_count_ ? thread_count_ : thread_count );
            
            RequireThreads( blks.BlockCount() );
            
            valprint("threads",blks.BlockCount());
            std::vector<std::future<T>> futures_( blks.BlockCount() );
            
            for( I thread = 0; thread < blks.BlockCount(); ++thread )
            {
                futures_[thread] = Submit(
                        std::forward<F>(fun),
                        thread, blks.begin(thread), blks.end(thread)
                );
            }
            
            T sum = static_cast<T>(0);
            
            for( I thread = 0; thread < thread_count_; ++thread )
            {
                sum += futures_[thread].get();
            }
            
            return sum;
        }
        
        
        
        template<typename I>
        JobPointers<I> CreateJobPointers( const I job_count )
        {
            return JobPointers<I>( job_count, static_cast<I>(thread_count) );
        }
        
        template<typename I>
        JobPointers<I> CreateJobPointers( const I job_count, I const * const costs, bool accumulate = true )
        {
            return JobPointers<I>( job_count, costs, static_cast<I>(thread_count), accumulate );
        }
        
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
            std::function<R()> fun = std::bind(
                std::forward<F>(task),
                std::forward<A>(args)...
            );
            
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
            for( Int thread = 0; thread < thread_count; ++thread )
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
            for( Int thread = 0; thread < thread_count; ++thread )
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
        std::atomic<size_t> tasks_total = 0;
        
        /**
         * @brief A mutex to synchronize access to the task queue by different threads.
         */
        mutable std::mutex tasks_mutex = {};
        
        /**
         * @brief The number of threads in the pool.
         */
        Int thread_count = 0;
        
        /**
         * @brief A smart pointer to manage the memory allocated for the threads.
         */
//        std::unique_ptr<std::thread[]> threads = nullptr;
        
        std::vector<std::thread> threads;
        
        std::vector<std::future<void>> futures;
        
        /**
         * @brief An atomic variable indicating that WaitForTasks() is active and expects to be notified whenever a task is done.
         */
        std::atomic<bool> waiting = false;
                         
                         
    }; // class ThreadPool

    inline ThreadPool thread_pool;
    
} // namespace Tools
