#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline void add_to_buffer( ptr<R> from, mut<S> to, const std::size_t n )
    {
        for( std::size_t i = 0; i < n; ++i )
        {
            to[i] += scalar_cast<S>(from[i]);
        }
    }
    
    template <typename R, typename S>
    force_inline void add_to_buffer( ptr<R> from, mut<S> to, const std::size_t n, const std::size_t thread_count )
    {
        ParallelDo(
            [=]( const std::size_t i )
            {
                to[i] += scalar_cast<S>(from[i]);
            },
            n,
            thread_count
        );
    }
    
    template <std::size_t n, typename R, typename S>
    force_inline void add_to_buffer( ptr<R> from, mut<S> to )
    {
        for( std::size_t i = 0; i < n; ++i )
        {
            to[i] += scalar_cast<S>(from[i]);
        }
    }
    
} // namespace Tools


