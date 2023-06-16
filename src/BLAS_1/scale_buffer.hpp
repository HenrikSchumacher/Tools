#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline void scale_buffer( const R beta_, mut<S> y, const Size_T n )
    {
        
        const auto beta = scalar_cast<S>(beta_);
        
        for( Size_T i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }
    
    template <typename R, typename S>
    force_inline void scale_buffer( const R beta_, mut<S> y, const Size_T n, const Size_T thread_count )
    {
        if( thread_count <= 1 )
        {
            scale_buffer( beta_, y, n );
        }
        else
        {
            const auto beta = scalar_cast<S>(beta_);
            
            ParallelDo(
                [=]( const Size_T i )
                {
                    y[i] *= beta;
                },
                n,
                thread_count
            );
        }
    }

    template <Size_T n, typename R, typename S>
    force_inline void scale_buffer( const R beta_, mut<S> y )
    {
        const auto beta = scalar_cast<S>(beta_);
        
        for( Size_T i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }

    
} // namespace Tools
