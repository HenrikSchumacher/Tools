#pragma once

// Playing around with designated initializers

namespace Tools
{
    
//    template <typename T>
//    struct Type_T {
//        using Type = T;
//    };
//    
//    template <typename T>
//    inline constexpr Type_T<T> Type{};

    
    struct Kron
    {
        struct TArg
        {
            Scalar::Flag a_flag = Scalar::Flag::Generic;
            Op opA = Op::Id; Size_T mA = VarSize; Size_T nA = VarSize;
            Op opB = Op::Id; Size_T mB = VarSize; Size_T nB = VarSize;
            Scalar::Flag b_flag = Scalar::Flag::Generic;
            Parallel_T parQ = Sequential;
        };
        
        template<TArg targ>
        struct Prod
        {
//            template <
//                typename A_T, typename B_T, typename C_T,
//                typename a_T = Scalar::Real<A_T>,
//                typename b_T = Scalar::Real<C_T>,
//            
//            >
//            struct MArg
//            {
//                a_T a = Scalar::One<a_T>; // TODO: Default this according to targ.
//                cptr<A_T> A;
////                Int mA = targ.mA;
////                Int nA = targ.nA;
////                Int ldA = nA;
//                cptr<B_T> B;
////                Int mB = targ.mB;
////                Int nB = targ.nB;
////                Int ldB = nB;
//                b_T b  = Scalar::Zero<b_T>; // TODO: Default this according to targ.
//                mptr<C_T> C;
////                Int ldC = nA * nB;
////                Int thread_count = 1; // TODO: Default this according to targ.
//            };
//            
//            // deduction guide
//            template <
//                typename A_T, typename B_T, typename C_T, typename a_T, typename b_T, typename I = Int
//            >
//            MArg( a_T, A_T *, B_T *, b_T, C_T * )
//            ->
//            MArg<A_T,B_T,C_T,a_T,b_T>;
//            
//            struct OArg
//            {
////                cref<a_T> a;
////                cptr<A_T> A;
//                Int mA = targ.mA;
//                Int nA = targ.nA;
//                Int ldA = nA;
////                cptr<B_T> B;
//                Int mB = targ.mB;
//                Int nB = targ.nB;
//                Int ldB = nB;
////                cref<b_T> b;
////                mptr<C_T> C;
//                Int ldC = nA * nB;
//                Int thread_count = 1;
//            };
//            
//
//            
//            template<
//                typename A_T, typename B_T, typename C_T, typename a_T, typename b_T
//            >
//            void operator()(
//                MArg<A_T,B_T,C_T,a_T,b_T> marg,
//                OArg oarg
//            )
//            {
//                constexpr Scalar::Flag A_flag =
//                    (targ.a_flag == Scalar::Flag::Zero)
//                    ? Scalar::Flag::Zero
//                    : Scalar::Flag::Generic;
//                
//                
//                // TODO: Currently we parallelize only over the first dimension of `A`. We have bad luck if that is small.
//                
//                Do<targ.nA,targ.parQ>(
//                    [=](const Int i )
//                    {
//                        const Int incC = oarg.ldC * oarg.mB * i;
//                        
//                        for( Int k = 0; k < oarg.mB; ++k )
//                        {
//                            outerprod_buffers<
//                                A_flag, targ.b_flag, targ.nA, targ.nB,
//                                Sequential, targ.opA, targ.opB
//                            >(
//                              marg.a, &marg.A[oarg.ldA * i],
//                                      &marg.B[oarg.ldB * k],
//                              marg.b, &marg.C[incC + oarg.ldC * k],
//                              oarg.nB, oarg.nA, oarg.nB
//                            );
//                        }
//                    },
//                    oarg.mA, oarg.thread_count
//                );
//            }
            
            
            
            
            
            
            template <
                typename A_T, typename B_T, typename C_T,
                typename a_T = Scalar::Real<A_T>,
                typename b_T = Scalar::Real<C_T>,
                typename Int = Size_T
            >
            struct Arg
            {
                a_T a = Scalar::One<a_T>; // TODO: Default this according to targ.
                cptr<A_T> A;
                Int mA = targ.mA;
                Int nA = targ.nA;
                Int ldA = nA;
                cptr<B_T> B;
                Int mB = targ.mB;
                Int nB = targ.nB;
                Int ldB = nB;
                b_T b  = Scalar::Zero<b_T>; // TODO: Default this according to targ.
                mptr<C_T> C;
                Int ldC = nA * nB;
                Int thread_count = 1; // TODO: Default this according to targ.
            };
            
            
            // deduction guide
            template <
                typename A_T, typename B_T, typename C_T,
                typename a_T, typename b_T, typename Int
            >
            Arg( a_T, A_T *, Int, Int, Int, B_T *, Int, Int, Int, b_T, C_T *, Int, Int )
            ->
            Arg<A_T,B_T,C_T,a_T,b_T,Int>;
            
            
            template<
                typename A_T, typename B_T, typename C_T,
                typename a_T, typename b_T, typename Int
            >
            void operator()( Arg<A_T,B_T,C_T,a_T,b_T,Int> arg )
            {
                
                static_assert( IntQ<Int>, "" );

                constexpr Scalar::Flag A_flag =
                    (targ.a_flag == Scalar::Flag::Zero)
                    ? Scalar::Flag::Zero
                    : Scalar::Flag::Generic;
                
                
                // TODO: Currently we parallelize only over the first dimension of `A`. We have bad luck if that is small.
                
                Do<targ.nA,targ.parQ>(
                    [=](const Int i )
                    {
                        const Int incC = arg.ldC * arg.mB * i;
                        
                        for( Int k = 0; k < arg.mB; ++k )
                        {
                            outerprod_buffers<
                                A_flag, targ.b_flag, targ.nA, targ.nB,
                                Sequential, targ.opA, targ.opB
                            >(
                              arg.a, &arg.A[arg.ldA * i],
                                      &arg.B[arg.ldB * k],
                              arg.b, &arg.C[incC + arg.ldC * k],
                              arg.nB, arg.nA, arg.nB
                            );
                        }
                    },
                    arg.mA, arg.thread_count
                );
            }
//
//            template<typename S, typename T>
//            void operator()( S arg, T oarg )
//            {
//                this->operator()( MArg{marg}, OArg{oarg} );
//            }

        };
    };
    
    struct KronProd_TArgs
    {
        Scalar::Flag a_flag = Scalar::Flag::Generic;
        Op opA = Op::Id; Size_T mA = VarSize; Size_T nA = VarSize;
        Op opB = Op::Id; Size_T mB = VarSize; Size_T nB = VarSize;
        Scalar::Flag b_flag = Scalar::Flag::Generic;
        Parallel_T parQ = Sequential;
    };
    
    template <
        KronProd_TArgs targs,
        typename a_T, typename A_T, typename B_T,
        typename b_T, typename C_T, typename Int
    >
    struct KronProd_Args
    {
        cref<a_T> a;
        cptr<A_T> A;
        Int mA = static_cast<Int>(targs.mA);
        Int nA = static_cast<Int>(targs.nA);
        Int ldA = nA;
        cptr<B_T> B;
        Int mB = static_cast<Int>(targs.mB);
        Int nB = static_cast<Int>(targs.nB);
        Int ldB = nB;
        cref<b_T> b;
        mptr<C_T> C;
        Int ldC = nA * nB;
        Int thread_count = 1;
    };
    
    /*! Computes C = a * op_x(x) * op_y(y)^T + b * A, where op_x and op_y may either be `Op::Id` or `Op::Conj`.
     *
     */
    
    template <
        KronProd_TArgs targs,
        typename a_T, typename A_T, typename B_T,
        typename b_T, typename C_T, typename Int = Size_T
    >
    TOOLS_FORCE_INLINE constexpr void KronProd(
        KronProd_Args<targs,a_T,A_T,B_T,b_T,C_T,Int> args
    )
    {
        static_assert( IntQ<Int>, "" );
        
        constexpr Scalar::Flag A_flag =
            (targs.a_flag == Scalar::Flag::Zero)
            ? Scalar::Flag::Zero
            : Scalar::Flag::Generic;
        
        
        // TODO: Currently we parallelize only over the first dimension of `A`. We have bad luck if that is small.
        
        Do<targs.nA,targs.parQ>(
            [=](const Int i )
            {
                const Int incC = args.ldC * args.mB * i;
                
                for( Int k = 0; k < args.mB; ++k )
                {
                    outerprod_buffers<
                        A_flag, targs.b_flag, targs.nA, targs.nB,
                        Sequential, targs.opA, targs.opB
                    >(
                      args.a, &args.A[args.ldA * i],
                              &args.B[args.ldB * k],
                      args.b, &args.C[incC + args.ldC * k],
                      args.nB, args.nA, args.nB
                    );
                }
            },
            args.mA, args.thread_count
        );
    }
        
} // namespace Tools
