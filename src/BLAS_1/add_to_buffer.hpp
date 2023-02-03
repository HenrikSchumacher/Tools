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
        if( thread_count <= 1 )
        {
            add_to_buffer(from, to, n);
        }
        else
        {
            #pragma omp parallel for num_threads( thread_count )
            for( std::size_t thread = 0; thread < thread_count; ++thread )
            {
                const std::size_t i_begin = JobPointer(n,thread_count,thread  );
                const std::size_t i_end   = JobPointer(n,thread_count,thread+1);
                
                for( std::size_t i = i_begin; i < i_end; ++i )
                {
                    to[i] += scalar_cast<S>(from[i]);
                }
            }
        }
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


