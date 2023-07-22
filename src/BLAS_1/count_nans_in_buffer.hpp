#pragma once

namespace Tools
{
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T
    >
    force_inline Size_T count_nans_in_buffer(
        mptr<T> a,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "count_nans_in_buffer", thread_count );
        
        if constexpr ( parQ == Parallel )
        {
            return DoReduce(
                [=]( const Size_T thread ) -> Size_T
                {
                    const Size_T i_begin = JobPointer( n, thread_count, thread     );
                    const Size_T i_end   = JobPointer( n, thread_count, thread + 1 );

                    Size_T counter = 0;
                    
                    for( Size_T i = i_begin; i < i_end; ++i )
                    {
                        counter += static_cast<Size_T>(std::isnan(a[i]));
                    }
                },
                AddReducer<Size_T,Size_T>(),
                Scalar::Zero<Size_T>,
                thread_count
            );
        }
        else if constexpr ( N == VarSize )
        {
            Size_T counter = 0;
            
            for( Size_T i = 0; i < n; ++i )
            {
                counter += static_cast<Size_T>(std::isnan(a[i]));
            }
            
            return counter;
        }
        else if constexpr ( N > VarSize )
        {
            Size_T counter = 0;
            
            for( Size_T i = 0; i < N; ++i )
            {
                counter += static_cast<Size_T>(std::isnan(a[i]));
            }
            
            return counter;
        }
    
    }
    
} // namespace Tools
