#pragma once

namespace Tools
{
    
    template <typename T>
    force_inline void iota_buffer( mut<T> a, const size_t n )
    {
        for( size_t i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
    template <typename T>
    force_inline void iota_buffer( mut<T> a, const size_t n, const size_t thread_count )
    {
        #pragma omp parallel for num_threads( thread_count )
        for( size_t i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
    template <size_t n, typename T>
    force_inline void iota_buffer( mut<T> a )
    {
        for( size_t i = 0; i < n; ++i )
        {
            a[i] = static_cast<T>(i);
        }
    }
    
} // namespace Tools
