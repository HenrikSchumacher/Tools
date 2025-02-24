#pragma once

namespace Tools
{
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T, typename Int = Size_T
    >
    TOOLS_FORCE_INLINE Int count_nans_in_buffer(
        mptr<T> a,
        const Int n = static_cast<Int>(N), const Int thread_count = 1
    )
    {
        check_sequential<parQ>( "count_nans_in_buffer", thread_count );
        
        if constexpr ( parQ == Parallel )
        {
            return DoReduce(
                [=]( const Int thread ) -> Size_T
                {
                    const Size_T i_begin = JobPointer( n, thread_count, thread     );
                    const Size_T i_end   = JobPointer( n, thread_count, thread + 1 );

                    Int counter = 0;
                    
                    for( Size_T i = i_begin; i < i_end; ++i )
                    {
                        counter += NaNQ(a[i]);
                    }
                    
                    return counter;
                },
                AddReducer<Int,Size_T>(),
                Scalar::Zero<Int>,
                thread_count
            );
        }
        else if constexpr ( N <= VarSize )
        {
            Size_T counter = 0;
            
            for( Size_T i = 0; i < n; ++i )
            {
                counter += NaNQ(a[i]);
            }
            
            return counter;
        }
        else if constexpr ( N > VarSize )
        {
            Size_T counter = 0;
            
            for( Size_T i = 0; i < N; ++i )
            {
                counter += NaNQ(a[i]);
            }
            
            return counter;
        }
    
    }
    
} // namespace Tools
