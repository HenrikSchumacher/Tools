#pragma once

namespace Tools
{
    
    template <typename T>
    force_inline void iota_buffer( mut<T> a, const std::size_t n )
    {
        for( std::size_t i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
    template <typename T>
    force_inline void iota_buffer( mut<T> a, const std::size_t n, const std::size_t thread_count )
    {
        ParallelDo(
            [=]( const std::size_t i )
            {
                a[i] = static_cast<T>(i);
            },
            n,
            thread_count
        );
    }
    
    template <std::size_t n, typename T>
    force_inline void iota_buffer( mut<T> a )
    {
        for( std::size_t i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
} // namespace Tools
