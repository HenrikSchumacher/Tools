#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T
    >
    force_inline void conjugate_buffer(
        mptr<x_T> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "conjugate_buffer", thread_count );
        
        if constexpr ( !Scalar::RealQ<x_T> )
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    x[i] = Conj(x[i]);
                },
                n, thread_count
            );
        }
    }

} // namespace Tools

