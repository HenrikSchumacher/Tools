#pragma once

namespace Tools
{
    template <
        Size_T M, Size_T N, Parallel_T parQ,
        typename X_T, typename Y_T
    >
    force_inline constexpr void copy_matrix(
        cptr<X_T> X, const Size_T ldX,
        mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        if( (ldX == n) && (ldY == n) )
        {
            // Consecutive buffer. Do a standard copy.
            
            if constexpr ( (M > VarSize) && (N > VarSize ) )
            {
                copy_buffer<M*N,Sequential>( X, Y );
            }
            else
            {
                copy_buffer<VarSize,Parallel>( X, Y, m * n, thread_count);
            }
        }
        else
        {
            Do<M,parQ,Static>(
                [=]( const Size_T i )
                {
                    copy_buffer<N,Sequential>( &X[ldX * i], &Y[ldY * i], n );
                },
                m, thread_count
            );
        }
    }
    
    // Overload for fixed sizes.
    template <
        Size_T M, Size_T N,
        typename X_T, typename Y_T
    >
    force_inline constexpr void copy_matrix(
        cptr<X_T> X, const Size_T ldX,
        mptr<Y_T> Y, const Size_T ldY
    )
    {
        static_assert( M > VarSize, "" );
        static_assert( N > VarSize, "" );
        
        copy_matrix<M,N,Sequential>( X, ldX, Y, ldY, M, N, Size_T(1) );
    }
    
    
    // Overload for variable sizes, parallel evaluation.
    template <
        typename X_T, typename Y_T
    >
    force_inline constexpr void copy_matrix(
        cptr<X_T> X, const Size_T ldX,
        mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        copy_matrix<VarSize,VarSize,Parallel>( X, ldX, Y, ldY, m, n, thread_count );
    }
    
    
    // Overload for variable sizes, sequential evaluation.
    template <
        typename X_T, typename Y_T
    >
    force_inline constexpr void copy_matrix(
        cptr<X_T> X, const Size_T ldX,
        mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n
    )
    {
        copy_matrix<VarSize,VarSize,Parallel>( X, ldX, Y, ldY, m, n, Size_T(1) );
    }
    
} // namespace Tools


//namespace Tools
//{
//    // cf. Tensor2::Write and Tensor2::Read.
//    template <
//        Size_T N = VarSize, Parallel_T parQ = Sequential, Op opx = Op::Id,
//        typename X_T, typename Y_T
//    >
//    force_inline constexpr void copy_matrix(
//        cptr<X_T> X, const Size_T ldX,
//        mptr<Y_T> Y, const Size_T ldY,
//        const Size_T m, const Size_T n = N, const Size_T thread_count = 1
//    )
//    {
//        // TODO: Parallelize over the greater dimension?
//        
//        if constexpr ( opx == Op::Id )
//        {
//            ParallelDo(
//                [=]( const Size_T i )
//                {
//                    copy_buffer<N>( &X[ldX * i], &Y[ldY * i], n );
//                },
//                m, thread_count
//            );
//        }
//        else if constexpr ( opx == Op::Conj )
//        {
//            ParallelDo(
//                [=]( const Size_T i )
//                {
//                    for( Size_T j = 0; j < ((N>VarSize) ? N : n); ++j )
//                    {
//                        Y[ldY * i + j] = scalar_cast<Y_T>(Conj(X[ldX * i + j]));
//                    }
//                },
//                m, thread_count
//            );
//        }
//        else if constexpr ( opx == Op::Trans )
//        {
//            // TODO: If n is small, then the parallelization here is inefficient.
//            ParallelDo(
//                [=]( const Size_T j )
//                {
//                    for( Size_T i = 0; i < m; ++i )
//                    {
//                        Y[ldY * i + j] = scalar_cast<Y_T>(X[ldX * j + i]);
//                    }
//                },
//                n, thread_count
//            );
//        }
//        else if constexpr ( opx == Op::ConjTrans )
//        {
//            // TODO: If n is small, then the parallelization here is inefficient.
//            ParallelDo(
//                [=]( const Size_T j )
//                {
//                    for( Size_T i = 0; i < m; ++i )
//                    {
//                        Y[ldY * i + j] = scalar_cast<Y_T>(Conj(X[ldX * j + i]));
//                    }
//                },
//                n, thread_count
//            );
//        }
//        else
//        {
//            eprint(
//                std::string("copy_matrix")
//                + "<" + ToString(N)
//                + "," + ToString(parQ)
//                + "," + TypeName<X_T>
//                + "," + TypeName<Y_T>
//                + ">(" + ToString(m) + "," + ToString(n) + ")::Write: No implementation for op available."
//            );
//        }
//    }
//}
