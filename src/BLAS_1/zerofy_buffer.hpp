#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T, typename Int = Size_T
    >
    force_inline constexpr void zerofy_buffer( 
        mptr<T> a, const Int n = N, const Int thread_count = 1
    )
    {
        check_sequential<parQ>( "zerofy_buffer", thread_count );
        
        static_assert( IntQ<Int>, "");
        
        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Sequential )
            {
                std::fill( &a[0], &a[n], static_cast<T>(0) );
            }
            else
            {
                if( thread_count <= Int(1) )
                {
                    std::fill( &a[0], &a[n], static_cast<T>(0) );
                }
                else
                {
                    ParallelDo(
                        [=]( const Size_T thread )
                        {
                            const Size_T begin = JobPointer(n,thread_count,thread  );
                            const Size_T end   = JobPointer(n,thread_count,thread+1);
                            
                            std::fill( &a[begin], &a[end], static_cast<T>(0) );
                        },
                        thread_count
                    );
                }
            }
        }
        else
        {
            std::fill( &a[0], &a[N], static_cast<T>(0) );
        }
        
    }
} // namespace Tools
