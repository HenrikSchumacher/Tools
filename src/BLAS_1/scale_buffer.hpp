#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline void scale_buffer( const R beta_, mut<S> y, const std::size_t n )
    {
        
        const auto beta = scalar_cast<S>(beta_);
        
        for( std::size_t i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }
    
    template <typename R, typename S>
    force_inline void scale_buffer( const R beta_, mut<S> y, const std::size_t n, const std::size_t thread_count )
    {
        if( thread_count <= 1 )
        {
            scale_buffer( beta_, y, n );
        }
        else
        {
            const auto beta = scalar_cast<S>(beta_);
            
            ParallelDo(
                [=]( const std::size_t i )
                {
                    y[i] *= beta;
                },
                n,
                thread_count
            );
        }
    }

    template <std::size_t n, typename R, typename S>
    force_inline void scale_buffer( const R beta_, mut<S> y )
    {
        const auto beta = scalar_cast<S>(beta_);
        
        for( std::size_t i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }

    
} // namespace Tools
