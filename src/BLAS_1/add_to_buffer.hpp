#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T
    >
    force_inline void add_to_buffer(
        cptr<x_T> x, mptr<y_T> y,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        combine_buffers<Scalar::Flag::Plus,Scalar::Flag::Plus,N,parQ,Op::Id,Op::Id>(
            Scalar::One<y_T>, x,
            Scalar::One<y_T>, y,
            n,thread_count
        );
    }
    
//    template <
//        Size_T N = VarSize, Parallel_T parQ = Sequential,
//        typename x_T, typename y_T
//    >
//    force_inline void add_to_buffer(
//        cptr<x_T> x, mptr<y_T> y,
//        const Size_T n = N, const Size_T thread_count = 1
//    )
//    {
//        check_sequential<parQ>( "add_to_buffer", thread_count );
//        
//        if constexpr ( (N > VarSize) && VectorizableQ<y_T> && SameQ<x_T,y_T> )
//        {
//            vec_T<N,y_T> x_vec;
//            vec_T<N,y_T> y_vec;
//            
//            // All this copying looks insane, but the clang compiler happily optimizes it away...
//            
//            copy_buffer<N>( x, reinterpret_cast<y_T *>(&x_vec) );
//            copy_buffer<N>( y, reinterpret_cast<y_T *>(&y_vec) );
//
//            y_vec += x_vec;
//
//            copy_buffer<N>( reinterpret_cast<y_T *>(&y_vec), y );
//        }
//        else
//        {
//            Do<N,parQ>(
//                [=]( const Size_T i )
//                {
//                    y[i] += scalar_cast<y_T>(x[i]);
//                },
//                n, thread_count
//            );
//        }
//    }

} // namespace Tools
