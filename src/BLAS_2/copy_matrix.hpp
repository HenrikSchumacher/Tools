#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename X_T, typename Y_T
    >
    force_inline constexpr void copy_matrix(
        cptr<X_T> X, const Size_T ldX,
        mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        if( (ldX == n) && (ldY == n) )
        {
            // Consecutive buffer. Do a standard copy.
            copy_buffer<VarSize,Parallel>( X, Y, m * n, thread_count);
        }
        else
        {
            Do<VarSize,parQ,Static>(
                [=]( const Size_T i )
                {
                    copy_buffer<N,Sequential>( &X[ldX * i], &Y[ldY * i], n );
                },
                m, thread_count
            );
        }
    }
}
