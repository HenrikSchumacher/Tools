#pragma once

namespace Tools
{
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_matrices(
        cref<a_T> a, cptr<x_T> X, const Size_T ldX,
        cref<b_T> b, mptr<y_T> Y, const Size_T ldY,
        const Size_T m,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        Do<VarSize,parQ,Static>(
            [=]( const Size_T i )
            {
                combine_buffers<a_flag,b_flag,N,Sequential,opx,opy>(
                    a, &X[ldX * i], b, &Y[ldY * i], n
                );
            },
            m, thread_count
        );
    }
}
