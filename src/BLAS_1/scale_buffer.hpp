#pragma once

namespace Tools
{
    
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename R, typename S
    >
    force_inline void scale_buffer(
        const R beta_, mptr<S> y, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "scale_buffer", thread_count );
        
        if constexpr ( (N > VarSize) && VectorizableQ<S> && SameQ<R,S> )
        {
            vec_T<N,S> y_vec;
            
            // All this copying looks insane, but the clang compiler happily optimizes it away...
            
            copy_buffer<N>( y, reinterpret_cast<S *>(&y_vec) );

            y_vec *= beta_;

            copy_buffer<N>( reinterpret_cast<S *>(&y_vec), y );
        }
        else
        {
            const auto beta = scalar_cast<S>(beta_);
            
            Do<VarSize,Parallel>(
                [=]( const Size_T i )
                {
                    y[i] *= beta;
                },
                n, thread_count
            );
        }
    }

    
} // namespace Tools
