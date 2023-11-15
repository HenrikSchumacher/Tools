#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T
    >
    force_inline constexpr void fill_buffer(
        mptr<T> a, const T init, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "fill_buffer", thread_count );
        
        if constexpr ( N == VarSize )
        {
            if constexpr ( parQ == Sequential )
            {
                std::fill_n( a, n, init );
            }
            else
            {
                if( thread_count <= Scalar::One<Size_T> )
                {
                    std::fill_n( a, n, init );
                }
                else
                {
                    ParallelDo(
                        [=]( const Size_T thread )
                        {
                            const Size_T begin = JobPointer(n,thread_count,thread  );
                            const Size_T end   = JobPointer(n,thread_count,thread+1);
                            
                            std::fill( &a[begin], &a[end], init );
                        },
                        thread_count
                    );
                }
            }
        }
        else
        {
            std::fill_n( a, N, init );
        }
        
    }
    
} // namespace Tools
