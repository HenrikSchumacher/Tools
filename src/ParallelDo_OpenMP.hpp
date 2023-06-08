#pragma once

namespace Tools
{
    
    // Executes the function `fun` of the form `[]( const Int thread ) -> void {...}` parallelized over `thread_count` threads.
    template<typename F, typename Int>
    force_inline void ParallelDo( F && fun, const Int thread_count )
    {
        if( thread_count <= static_cast<Int>(1) )
        {
            std::invoke( fun, static_cast<Int>(0) );
        }
        else
        {
            #pragma omp parallel for num_threads( thread_count) schedule( static )
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                std::invoke( fun, thread );
            }
        }
    }
    
    // Executes the function `fun` of the `form []( const Int thread ) -> void {...}` parallelized over `thread_count` threads.
    // Afterwards, reduces with the function `reducer` of the form `[]( const Int thread, const S &, T & result ) {...}`.
    template<typename F, typename R, typename T, typename Int>
    force_inline T ParallelDoReduce( F && fun, R && reducer, const T & init, const Int thread_count )
    {
        T result ( init );
        
        if( thread_count <= static_cast<Int>(1) )
        {
            std::invoke(
                reducer,
                static_cast<Int>(0),
                std::invoke( fun, static_cast<Int>(0) ),
                result
            );
        }
        else
        {
            #pragma omp parallel for num_threads( thread_count) schedule( static )
            for( Int thread = 0; thread < thread_count; ++thread )
            {
                T local_result = std::invoke( fun, thread );
                
                #pragma omp critical ( ParallelDoReduce )
                {
                    std::invoke( reducer, thread, local_result, result );
                }
            }
        }
        
        return result;
    }
    
}
