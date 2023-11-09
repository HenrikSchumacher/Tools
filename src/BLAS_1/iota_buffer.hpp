#pragma once

namespace Tools
{
    
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T
    >
    force_inline constexpr void iota_buffer(
        mptr<T> a,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "iota_buffer", thread_count );
        
        Do<N,parQ>(
            [=]( const Size_T i )
            {
                a[i] = static_cast<T>(i);
            },
            n, thread_count
        );
    }
    
} // namespace Tools
