#pragma once

namespace Tools
{
    struct AndReducer
    {
        void operator()( const Size_T thread, cref<bool> value, mref<bool> result )
        {
            (void)thread;
            
            result = result && value;
        }
    };
    
    struct OrReducer
    {
        void operator()( const Size_T thread, cref<bool> value, mref<bool> result )
        {
            (void)thread;
            
            result = result || value;
        }
    };
    
    template<typename Scal_in, typename Scal_out>
    struct AddReducer
    {
        void operator()( const Size_T thread, cref<Scal_in> value, mref<Scal_out> result )
        {
            (void)thread;
            
            result += value;
        }
    };
    
    template<typename Scal_in, typename Scal_out>
    struct TimesReducer
    {
        void operator()( const Size_T thread, cref<Scal_in> value, mref<Scal_out> result )
        {
            (void)thread;
            
            result *= value;
        }
    };
    
    
    //ParallelDo    
    
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` on the range [begin,end[, parallelized over `thread_count` threads.
    template<typename F, typename Int>
    TOOLS_FORCE_INLINE void ParallelDo(
        F && fun, const Int begin, const Int end, const Int thread_count
    )
    {
        static_assert(IntQ<Int>,"");
        
        ParallelDo(
            [&fun,begin,end,thread_count]( const Int thread )
            {
                const Int i_begin = begin + JobPointer( end - begin, thread_count, thread     );
                const Int i_end   = begin + JobPointer( end - begin, thread_count, thread + 1 );

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
    TOOLS_FORCE_INLINE void ParallelDo(
        F && fun, const Int n, const Int thread_count
    )
    {
        static_assert(IntQ<Int>,"");
        
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
    
    
    template<typename F, typename R, typename T, typename Int>
    TOOLS_FORCE_INLINE T ParallelDoReduce(
        F && fun, R && reducer, cref<T> init, 
        const Int begin, const Int end, const Int thread_count
    )
    {
        static_assert(IntQ<Int>,"");
        
        return ParallelDoReduce(
            [&fun,&reducer,begin,end,thread_count,&init]( const Int thread ) -> T
            {
                T result (init);

                const Int i_begin = begin + JobPointer( end - begin, thread_count, thread     );
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
    
//    template<typename F, typename R, typename T, typename Int>
//    TOOLS_FORCE_INLINE T ParallelDoReduce(
//        F && fun, R && reducer, cref<T> init, const Int n, const Int thread_count
//    )
//    {
//        return ParallelDoReduce(
//            std::forward<F>(fun),
//            std::forward<R>(reducer),
//            init, Int(0), n, thread_count
//        );
//    }
    


}
