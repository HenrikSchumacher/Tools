#pragma once

#include <thread>
#include <future>

namespace Tools
{   
    // Executes the function `fun` of the form `[]( const Int thread ) -> S {...}` parallelized over `thread_count` threads.
    template<typename F, typename Int = Size_T>
    force_inline void ParallelDo( F && fun, const Int thread_count )
    {
        static_assert(IntQ<Int>,"");
        
        if( thread_count <= Int(1) )
        {
            std::invoke( fun, Int(0) );
        }
        else
        {
            std::vector<std::future<void>> futures ( static_cast<Size_T>(thread_count) );
            
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                futures[static_cast<Size_T>(thread)] = std::async( std::forward<F>(fun), thread );
            }
            
            for( auto & future : futures )
            {
                future.get();
            }
        }
    }
    
    // Executes the function `fun` of the `form []( const Int thread ) -> S {...}` parallelized over `thread_count` threads.
    // Afterwards, reduces with the function `reducer` of the form `[]( const Int thread, cref<S> value, mref<T> result ) {...}`.
    template<typename F, typename R, typename T, typename Int = Size_T>
    force_inline T ParallelDoReduce( F && fun, R && reducer, cref<T> init, const Int thread_count )
    {
        static_assert(IntQ<Int>,"");
        
        T result (init);
        
        if( thread_count <= Int(1) )
        {
            std::invoke( reducer, Int(0), fun(Int(0)), result );
        }
        else
        {
            std::vector<std::future<T>> futures ( static_cast<Size_T>(thread_count) );
            
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                futures[static_cast<Size_T>(thread)] = std::async( std::forward<F>(fun), thread );
            }
            
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                std::invoke( reducer, thread, futures[static_cast<Size_T>(thread)].get(), result );
            }
        }
        
        return result;
    }
    
    
    // Executes the function `fun` of the form `[]( const Int thread, const Int i ) -> void {...}` parallelized over `thread_count` threads.
    template<typename F, typename Int = Size_T>
    force_inline void ParallelDo_Dynamic(
        F && fun, const Int begin, const Int end, const Int inc, const Int thread_count
    )
    {
        static_assert(IntQ<Int>,"");
        
        if( end <= begin )
        {
            return;
        }
        
        if( thread_count <= Int(1) )
        {
            for( Int i = begin; i < end; ++i )
            {
                std::invoke( fun, Int(0), i );
            }
        }
        else
        {
            // Global index that is supposed to run from begin to end; shared by all threads.
            Int iter = begin;
            // A mutex to synchronize iter.
            std::mutex iter_mutex;
            
            // A vector to hold the workers.
            std::vector<std::future<void>> futures ( thread_count );
            
            // Lambda-function representing a single worker.
            auto worker = [&,end]( const Int thread )
            {
                bool done = false;
                
                // This worker is supposed to repeatedly check whether `iter` has run out of bounds.
                while( !done )
                {
                    Int i_begin = end;
                    Int i_end   = end;
                    
                    {
                        const std::lock_guard<std::mutex> lock_iter( iter_mutex );
                    
                        if( iter < end )
                        {   
                            // Iterator `iter` is not out of bound. Assign a new chunk to worker.
                            i_begin = iter;
                            
                            iter = Min( iter + inc, end );
                            
                            i_end   = iter;
                        }
                        else
                        {
                            // Nothing left to do. Stop here.
                            done = true;
                            
                            TOOLS_DEBUG_PRINT( "ParallelDo_Dynamic: " + ToString(thread) + " done." );
                            
                            return;
                        }
                    }
                    
                    TOOLS_DEBUG_PRINT( "ParallelDo_Dynamic: thread " + ToString(thread) + " processes tasks [" + ToString(i_begin) + "," + ToString(i_end)  + "[." );
                    // Let the worker process its assigned chunk.
                    for( Int i = i_begin; i < i_end; ++i )
                    {
                        fun( thread, i );
                    }
                }
            };
            
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                futures[static_cast<Size_T>(thread)] = std::async( worker, thread );
            }
            
            for( auto & future : futures )
            {
                future.get();
            }
        }
    }
    
}
