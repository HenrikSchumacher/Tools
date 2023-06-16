#pragma once

namespace Tools
{
    
    template <typename T>
    force_inline void iota_buffer( mut<T> a, const Size_T n )
    {
        for( Size_T i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
    template <typename T>
    force_inline void iota_buffer( mut<T> a, const Size_T n, const Size_T thread_count )
    {
        ParallelDo(
            [=]( const Size_T i )
            {
                a[i] = static_cast<T>(i);
            },
            n,
            thread_count
        );
    }
    
    template <Size_T n, typename T>
    force_inline void iota_buffer( mut<T> a )
    {
        for( Size_T i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
} // namespace Tools
