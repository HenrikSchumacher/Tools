#pragma once

namespace Tools
{
    template <Size_T N = VarSize, Parallel_T parQ = Sequential, typename X_T>
    force_inline constexpr void zerofy_matrix(
        mptr<X_T> X, const Size_T ldX,
        const Size_T m, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        Do<VarSize,parQ,Static>(
            [=]( const Size_T i )
            {
                zerofy_buffer<N,Sequential>( &X[ldX * i], n );
            },
            m, thread_count
        );
    }
}

