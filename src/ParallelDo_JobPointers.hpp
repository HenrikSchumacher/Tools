#pragma once


namespace Tools
{
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` parallelized over chunks defined by `job_ptr`.
    template<typename F, IntQ Int>
    TOOLS_FORCE_INLINE void ParallelDo( F && fun, cref<JobPointers<Int>> job_ptr )
    {
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
    
    template<Size_T N, Parallel_T parQ = Parallel, typename F, IntQ Int>
    TOOLS_FORCE_INLINE void Do( F && fun, cref<JobPointers<Int>> job_ptr )
    {
        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Parallel )
            {
                ParallelDo( std::forward<F>(fun), job_ptr );
            }
            else
            {
                Do<VarSize,Sequential>( std::forward<F>(fun), job_ptr.JobCount() );
            }
        }
        else
        {
            Do<N,Sequential>( std::forward<F>(fun), N );
        }
    }
    
    template<Parallel_T parQ = Parallel, typename F, IntQ Int>
    TOOLS_FORCE_INLINE void Do( F && fun, cref<JobPointers<Int>> job_ptr )
    {
        Do<VarSize,parQ>( std::forward<F>(fun), job_ptr );
    }
    
    
    template<typename F, typename R, typename T, IntQ Int>
    TOOLS_FORCE_INLINE T ParallelDoReduce(
        F && fun, R && reducer, cref<T> init, cref<JobPointers<Int>> job_ptr
    )
    {
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
    
    template<
        Size_T N, Parallel_T parQ = Parallel,
        typename F, typename R, typename T, IntQ Int
    >
    TOOLS_FORCE_INLINE void DoReduce( F && fun, R && reducer, cref<T> init, cref<JobPointers<Int>> job_ptr )
    {
        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Parallel )
            {
                ParallelDoReduce( std::forward<F>(fun), std::forward<R>(reducer), init, job_ptr );
            }
            else
            {
                DoReduce<VarSize,Sequential>( std::forward<F>(fun), std::forward<R>(reducer), init, job_ptr.JobCount() );
            }
        }
        else
        {
            DoReduce<N,Sequential>( std::forward<F>(fun), std::forward<R>(reducer), init, N );
        }
    }
    
    
    template<Parallel_T parQ = Parallel, typename F, typename R, typename T, IntQ Int>
    TOOLS_FORCE_INLINE void DoReduce(
        F && fun, R && reducer, cref<T> init, cref<JobPointers<Int>> job_ptr
    )
    {
        DoReduce( std::forward<F>(fun), std::forward<R>(reducer), init, job_ptr );
    }
}
