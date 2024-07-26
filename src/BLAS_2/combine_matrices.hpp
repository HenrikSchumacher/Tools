#pragma once

namespace Tools
{
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Size_T M, Size_T N, Parallel_T parQ,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_matrices(
        cref<a_T> a, cptr<x_T> X, const Size_T ldX,
        cref<b_T> b, mptr<y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        Do<M,parQ,Static>(
            [=]( const Size_T i )
            {
                combine_buffers<a_flag,b_flag,N,Sequential,opx,opy>(
                    a, &X[ldX * i], b, &Y[ldY * i], n
                );
            },
            m, thread_count
        );
    }
    
    // Overload for fixed sizes.
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Size_T M, Size_T N,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_matrices(
        cref<a_T> a, cptr<x_T> X, const Size_T ldX,
        cref<b_T> b, mptr<y_T> Y, const Size_T ldY
    )
    {
        static_assert( M > VarSize, "" );
        static_assert( N > VarSize, "" );
        
        combine_matrices<a_flag,b_flag,M,N,Sequential,opx,opy>(
            a, X, ldX,
            b, Y, ldY,
            M, N, Size_T(1)
         );
    }
    
    
    // Overload for variable sizes, parallel evaluation.
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_matrices(
        cref<a_T> a, cptr<x_T> X, const Size_T ldX,
        cref<b_T> b, mptr<y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        combine_matrices<a_flag,b_flag,VarSize,VarSize,Parallel,opx,opy>(
            a, X, ldX,
            b, Y, ldY,
            m, n, thread_count
         );
    }
    
    
    // Overload for variable sizes, sequential evaluation.
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_matrices(
        cref<a_T> a, cptr<x_T> X, const Size_T ldX,
        cref<b_T> b, mptr<y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n
    )
    {
        combine_matrices<a_flag,b_flag,VarSize,VarSize,Sequential,opx,opy>(
            a, X, ldX,
            b, Y, ldY,
            m, n, Size_T(1)
         );
    }
    
} // namespace Tools
