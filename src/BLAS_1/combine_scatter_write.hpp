#pragma once

namespace Tools
{
    
    template<
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename R_0, typename S_0, typename Int, typename R_1, typename S_1
    >
    constexpr force_inline void combine_scatter_write(
        cref<R_0> alpha, cptr<S_0> x,
        cref<R_1> beta,  mptr<S_1> y, cptr<Int> idx, Size_T n = N, Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "combine_scatter_write", thread_count );
        
        using namespace Scalar;
        
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

        static_assert( ComplexQ<S_1> || (RealQ<R_0> && RealQ<S_0> && RealQ<R_1>),
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `alpha` and `beta` to the right precision because this is better done once before any loop that calls `combine_buffers`. Hence we prefer a compile error here.
        
        static_assert(
            Prec<R_0> == Prec<S_1>,
            "Precisions of first and fourth argument do not coincide."
        );
        
        static_assert(
            Prec<R_1> == Prec<S_1>,
            "Precisions of third and fourth argument do not coincide."
        );
        
        Do<N,parQ,Static>(
            [=]( const Size_T k )
            {
                combine_scalars<alpha_flag,beta_flag>( alpha, x[k], beta, y[idx[k]] );
            },
            n, thread_count
        );
    }

} // namespace Tools

