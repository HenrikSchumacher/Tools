#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename R, typename S
    >
    force_inline void add_to_buffer(
        cptr<R> x, mptr<S> y,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "add_to_buffer", thread_count );
        
        if constexpr ( (N > VarSize) && VectorizableQ<S> && SameQ<R,S> )
        {
            vec_T<N,S> x_vec;
            vec_T<N,S> y_vec;
            
            // All this copying looks insane, but the clang compiler happily optimizes it away...
            
            copy_buffer<N>( x, reinterpret_cast<S *>(&x_vec) );
            copy_buffer<N>( y, reinterpret_cast<S *>(&y_vec) );

            y_vec += x_vec;

            copy_buffer<N>( reinterpret_cast<S *>(&y_vec), y );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    y[i] += scalar_cast<S>(x[i]);
                },
                n, thread_count
            );
        }
    }

} // namespace Tools
