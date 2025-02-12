#pragma once


namespace Tools
{
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` parallelized over chunks defined by `job_ptr`.
    template<typename F, typename Int>
    force_inline void ParallelDo( F && fun, cref<JobPointers<Int>> job_ptr )
    {
        static_assert(IntQ<Int>,"");
        
        ParallelDo(
            [&fun,&job_ptr]( const Int thread )
            {
                const Int i_begin = job_ptr[thread  ];
                const Int i_end   = job_ptr[thread+1];

                for( Int i = i_begin; i < i_end; ++i )
                {
                    fun(i);
                }
            },
            job_ptr.ThreadCount()
        );
    }
    
    
    template<typename F, typename R, typename T, typename Int>
    force_inline T ParallelDoReduce(
        F && fun, R && reducer, cref<T> init, cref<JobPointers<Int>> job_ptr
    )
    {
        static_assert(IntQ<Int>,"");
        
        return ParallelDoReduce(
            [&fun,&reducer,&job_ptr,&init]( const Int thread ) -> T
            {
                T result (init);
                
                const Int i_begin = job_ptr[thread  ];
                const Int i_end   = job_ptr[thread+1];

                for( Int i = i_begin; i < i_end; ++i )
                {
                    std::invoke( reducer, thread, fun(i), result );
                }
                
                return result;
            },
            std::forward<R>(reducer),
            init,
            job_ptr.ThreadCount()
        );
    }
}
