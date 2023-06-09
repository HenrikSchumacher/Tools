#pragma once

#include "ThreadPool.hpp"

namespace Tools
{
    
    // Executes the function `fun` of the form `[]( const Int thread ) -> void {...}` parallelized over `thread_count` threads.
    template<typename F, typename Int>
    force_inline void ParallelDo( F && fun, const Int thread_count )
    {
        thread_pool.Do( std::forward<F>(fun), thread_count );
    }
    
    // Executes the function `fun` of the `form []( const Int thread ) -> void {...}` parallelized over `thread_count` threads.
    // Afterwards, reduces with the function `reducer` of the form `[]( const Int thread, const S &, T & result ) {...}`.
    template<typename F, typename R, typename T, typename Int>
    force_inline T ParallelDoReduce( F && fun, R && reducer, const T & init, const Int thread_count )
    {
        return thread_pool.DoReduce(
            std::forward<F>(fun), std::forward<R>(reducer), init, thread_count
        );
    }
    
}
