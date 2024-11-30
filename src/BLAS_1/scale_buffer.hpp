#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename a_T, typename x_T
    >
    force_inline void scale_buffer(
        cref<a_T> a, mptr<x_T> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        {
            #pragma float_control(precise, off)
            
            modify_buffer<Scalar::Flag::Generic,N,parQ,Op::Id>(a,x,n,thread_count);
        }
    }
    
//    template <
//        Size_T N = VarSize, Parallel_T parQ = Sequential,
//        typename b_T, typename y_T
//    >
//    force_inline void scale_buffer(
//        const b_T b_, mptr<y_T> y, const Size_T n = N, const Size_T thread_count = 1
//    )
//    {
//        check_sequential<parQ>( "scale_buffer", thread_count );
//        
//        const auto b = scalar_cast<y_T>(b_);
//        
//        if constexpr ( (N > VarSize) && VectorizableQ<y_T> )
//        {
//            vec_T<N,y_T> y_vec;
//            
//            // All this copying looks insane, but the clang compiler happily optimizes it away...
//            
//            copy_buffer<N>( y, reinterpret_cast<y_T *>(&y_vec) );
//
//            y_vec *= b;
//
//            copy_buffer<N>( reinterpret_cast<y_T *>(&y_vec), y );
//        }
//        else
//        {
//            Do<VarSize,Parallel>(
//                [=]( const Size_T i )
//                {
//                    y[i] *= b;
//                },
//                n, thread_count
//            );
//        }
//    }

    
} // namespace Tools
