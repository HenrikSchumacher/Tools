#pragma once

namespace Tools
{
    struct AndReducer
    {
        void operator()( const Size_T thread, const bool value, bool & result )
        {
            result = result && value;
        }
    };
    
    struct OrReducer
    {
        void operator()( const Size_T thread, const bool value, bool & result )
        {
            result = result || value;
        }
    };
    
    template<typename Scal_in, typename Scal_out>
    struct AddReducer
    {
        void operator()( const Size_T thread, const Scal_in value, Scal_out & result )
        {
            result += value;
        }
    };
    
    template<typename Scal_in, typename Scal_out>
    struct TimesReducer
    {
        void operator()( const Size_T thread, const Scal_in value, Scal_out & result )
        {
            result *= value;
        }
    };
    
    
    //ParallelDo
    
    
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` parallelized over chunks defined by `job_ptr`.
    template<typename F, typename Int>
    force_inline void ParallelDo(
        F && fun, const JobPointers<Int> & job_ptr
    )
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
    
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` on the range [begin,end[, parallelized over `thread_count` threads.
    template<typename F, typename Int>
    force_inline void ParallelDo(
        F && fun, const Int begin, const Int end, const Int thread_count
    )
    {
        ParallelDo(
            [&fun,begin,end,thread_count]( const Int thread )
            {
                const Int i_begin = begin + JobPointer( end - begin , thread_count, thread     );
                const Int i_end   = begin + JobPointer( end - begin , thread_count, thread + 1 );

                for( Int i = i_begin; i < i_end; ++i )
                {
                    fun(i);
                }
            },
            thread_count
        );
    }
    
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` on the range [0,end[, parallelized over `thread_count` threads.
    template<typename F, typename Int>
    force_inline void ParallelDo(
        F && fun, const Int n, const Int thread_count
    )
    {
        ParallelDo(
            [&fun,n,thread_count]( const Int thread )
            {
                const Int i_begin = JobPointer( n, thread_count, thread     );
                const Int i_end   = JobPointer( n, thread_count, thread + 1 );

                for( Int i = i_begin; i < i_end; ++i )
                {
                    fun(i);
                }
            },
            thread_count
        );
    }
    
//    template<typename F, typename R, typename T, typename Int>
//    force_inline void ParallelDo(
//        F && fun, R && reducer, const T & init, const Int n, const Int thread_count
//    )
//    {
//        ParallelDoReduce(
//            [&fun,&reducer,n,thread_count,&init]( const Int thread )
//            {
//                T result (init);
//
//                const Int i_begin = JobPointer( n, thread_count, thread     );
//                const Int i_end   = JobPointer( n, thread_count, thread + 1 );
//
//                for( Int i = i_begin; i < i_end; ++i )
//                {
//                    std::invoke( reducer, thread, fun(i), result );
//                }
//
//                return result;
//            },
//            std::forward<R>(reducer),
//            init,
//            thread_count
//        );
//    }
    
    
    //ParallelDoReduce
    
    template<typename F, typename R, typename T, typename Int>
    force_inline T ParallelDoReduce(
        F && fun, R && reducer, const T & init, const JobPointers<Int> & job_ptr
    )
    {
        return ParallelDoReduce(
            [&fun,&reducer,&job_ptr,&init]( const Int thread )
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
    
    template<typename F, typename R, typename T, typename Int>
    force_inline T ParallelDoReduce(
        F && fun, R && reducer, const T & init, const Int begin, const Int end, const Int thread_count
    )
    {
        return ParallelDoReduce(
            [&fun,&reducer,begin,end,thread_count,&init]( const Int thread )
            {
                T result (init);

                const Int i_begin = begin + JobPointer( end - begin, thread_count, thread      );
                const Int i_end   = begin + JobPointer( end - begin, thread_count, thread + 1 );

                for( Int i = i_begin; i < i_end; ++i )
                {
                    std::invoke( reducer, thread, fun(i), result );
                }

                return result;
            },
            std::forward<R>(reducer),
            init,
            thread_count
        );
    }
    


}
