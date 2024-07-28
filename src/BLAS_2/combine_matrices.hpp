#pragma once

namespace Tools
{
    /*!
     * @brief Computes Y = alpha * opx(X) + beta * opy(Y), where `X` and `Y`
     * are matrices of size `m` x `n` and `alpha` and `beta` are scalars.
     *
     * @tparam a_flag Specify code optimizations for the scalar `alpha`.
     *
     * @tparam b_flag Specify code optimizations for the scalar `beta`.
     *
     * @tparam M Compile-time knowledge of the number of rows.
     *   `M > 0` asks to use fixed-size loops that can be optimized by the compiler.
     *   `M = 0` uses variable the run-time row count specified by argument `m`.
     *
     * @tparam N Compile-time knowledge of the number of columns.
     *   `N > 0` : Use fixed-size loops that can be optimized by the compiler.
     *   `N = 0` : Use variable the run-time column count specified by argument `n`.
     *
     * @tparam parQ Whether to parallelize or not.
     *
     * @tparam opx Apply a scalar transformation to `X`. Possible values:
     *   `opx = Op::In`   : Apply the identity to the elements of `X`.
     *   `opx = Op::Conj` : Apply `std::Conj` to the elements of `X`.
     *
     * @tparam opy Apply a scalar transformation to `Y`. Possible values:
     *   `opy = Op::In`   : Apply the idenity to the elements of `Y`.
     *   `opy = Op::Conj` : Apply `std::Conj` to the elements of `Y`.
     *
     * @param alpha A scalar.
     *
     * @param X A matrix of size `m` x `n`.
     *
     * @param ldX Leading dimension of `X`.
     *
     * @param beta A scalar.
     *
     * @param Y A matrix of size `m` x `n`.
     *
     * @param ldY Leading dimension of `Y`.
     *
     * @param m Number of rows.
     *
     * @param n Number of columns.
     *
     * @param thread_count Number of threads to parallelize over.
     */
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Size_T M, Size_T N, Parallel_T parQ,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_matrices(
        const a_T alpha, cptr<x_T> X, const Size_T ldX,
        const b_T beta , mptr<y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        Do<M,parQ,Static>(
            [=]( const Size_T i )
            {
                combine_buffers<a_flag,b_flag,N,Sequential,opx,opy>(
                    alpha, &X[ldX * i], beta, &Y[ldY * i], n
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
        const a_T alpha, cptr<x_T> X, const Size_T ldX,
        const b_T beta , mptr<y_T> Y, const Size_T ldY
    )
    {
        static_assert( M > VarSize, "" );
        static_assert( N > VarSize, "" );
        
        combine_matrices<a_flag,b_flag,M,N,Sequential,opx,opy>(
            alpha, X, ldX,
            beta , Y, ldY,
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
        const a_T alpha, cptr<x_T> X, const Size_T ldX,
        const b_T beta , mptr<y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        combine_matrices<a_flag,b_flag,VarSize,VarSize,Parallel,opx,opy>(
            alpha, X, ldX,
            beta , Y, ldY,
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
        const a_T alpha, cptr<x_T> X, const Size_T ldX,
        const b_T beta , mptr<y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n
    )
    {
        combine_matrices<a_flag,b_flag,VarSize,VarSize,Sequential,opx,opy>(
            alpha, X, ldX,
            beta , Y, ldY,
            m, n, Size_T(1)
         );
    }
    
} // namespace Tools