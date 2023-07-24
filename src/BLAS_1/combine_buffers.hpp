#pragma once

namespace Tools
{
    
    
    
    template<
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        Op opx = Op::Id, Op opy = Op::Id,
        typename alpha_T, typename x_T, typename beta_T, typename y_T
    >
    force_inline void combine_buffers(
        cref<alpha_T> alpha, cptr<x_T> x,
        cref< beta_T> beta,  mptr<y_T> y,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "combine_buffers", thread_count );
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "combine_buffers: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "combine_buffers: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == Flag::Zero,  then it assumes alpha = 0.
        // If alpha_flag == Flag::Plus,  then it assumes alpha = 1.
        // If alpha_flag == Flag::Minus, then it assumes alpha = -1.
        // If alpha_flag == Flag::Generic, then it assumes generic values for alpha.
        
        // If beta_flag == Flag::Zero,  then it assumes beta = 0.
        // If beta_flag == Flag::Plus,  then it assumes beta = 1.
        // If beta_flag == Flag::Minus, then it assumes beta = -1.
        // If beta_flag == Flag::Generic, then it assumes generic values for beta.
        
        static_assert( ComplexQ<y_T> || (RealQ<alpha_T> && RealQ<x_T> && RealQ<beta_T>),
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `alpha` and `beta` to the right precision because this is better done once before any loop that calls `combine_buffers`. Hence we prefer a compile error here.
        
        static_assert(
            Prec<alpha_T> == Prec<y_T>,
            "Precisions of first and fourth argument do not coincide."
        );
        
        static_assert(
            Prec<beta_T> == Prec<y_T>,
            "Precisions of third and fourth argument do not coincide."
        );
        
        if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Zero) )
        {
            zerofy_buffer<N,parQ>(y,n,thread_count);
        }
        else if constexpr (
            (beta_flag == Flag::Zero) && (alpha_flag == Flag::Plus)
            && ( opx == Op::Id ) && (opy == Op::Id )
        )
        {
            copy_buffer<N,parQ>(x,y,n,thread_count);
        }
        else
        {
            if constexpr ( (N > VarSize) && VectorizableQ<y_T> )
            {
                vec_T<N,y_T> x_vec;
                vec_T<N,y_T> y_vec;
                
                // All this copying looks insane, but the clang compiler happily optimizes it away...
                
                copy_buffer<N>( x, reinterpret_cast<y_T *>(&x_vec) );
                copy_buffer<N>( y, reinterpret_cast<y_T *>(&y_vec) );

                y_vec = static_cast<y_T>(alpha) * x_vec + static_cast<y_T>(beta) * y_vec;

                copy_buffer<N>( reinterpret_cast<y_T *>(&y_vec), y );
                
            }
            else
            {
                Do<N,parQ,Static>(
                    [=]( const Size_T i )
                    {
                        combine_scalars<alpha_flag,beta_flag,opx,opy>( alpha, x[i], beta, y[i] );
                    },
                    n, thread_count
                );
            }
        }
    }

} // namespace Tools
