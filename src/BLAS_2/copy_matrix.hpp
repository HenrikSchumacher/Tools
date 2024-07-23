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
        Do<VarSize,parQ,Static>(
            [=]( const Size_T i )
            {
                copy_buffer<N,Sequential>( &X[ldX * i], &Y[ldY * i], n );
            },
            m, thread_count
        );
    }
}


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
