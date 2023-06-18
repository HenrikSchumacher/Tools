#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline void add_to_buffer( ptr<R> from, mut<S> to, const Size_T n )
    {
        for( Size_T i = 0; i < n; ++i )
        {
            to[i] += scalar_cast<S>(from[i]);
        }
    }
    
    template <typename R, typename S>
    force_inline void add_to_buffer( ptr<R> from, mut<S> to, const Size_T n, const Size_T thread_count )
    {
        ParallelDo(
            [=]( const Size_T i )
            {
                to[i] += scalar_cast<S>(from[i]);
            },
            n,
            thread_count
        );
    }
    
    template <Size_T n, typename R, typename S>
    force_inline void add_to_buffer( ptr<R> from, mut<S> to )
    {
        for( Size_T i = 0; i < n; ++i )
        {
            to[i] += scalar_cast<S>(from[i]);
        }
    }
    
} // namespace Tools
