#pragma once

namespace Tools
{
    template<
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    static force_inline constexpr void combine_buffers(
        const R_0 & alpha, ptr<S_0> x,
        const R_1 & beta,  mut<S_1> y,
        const std::size_t n
    )
    {
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
        
        static_assert( IsComplex<S_1> || (IsReal<R_0> && IsReal<S_0> && IsReal<R_1>),
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
        
        if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Zero) )
        {
            zerofy_buffer(y,n);
        }
        else if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Plus) )
        {
            copy_buffer(x,y,n);
        }
        else
        {
//            for( std::size_t k = 0; k < n; ++k )
//            {
//                y[k] = alpha * x[k] + beta * y[k];
//            }
            
            for( std::size_t k = 0; k < n; ++k )
            {
                combine_scalars<alpha_flag,beta_flag>(alpha, x[k], beta, y[k]);
            }
        }
    }
    
    
    template<
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline void combine_buffers(
        const R_0 & alpha, ptr<S_0> x,
        const R_1 & beta,  mut<S_1> y,
        const std::size_t n,
        const std::size_t thread_count
    )
    {
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
        
        static_assert( IsComplex<S_1> || (IsReal<R_0> && IsReal<S_0> && IsReal<R_1>),
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
        
        if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Zero) )
        {
            zerofy_buffer(y,n,thread_count);
        }
        else if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Plus) )
        {
            copy_buffer(x,y,n,thread_count);
        }
        else
        {
            if( thread_count > 1 )
            {
                #pragma omp parallel for num_threads( thread_count )
                for( std::size_t thread = 0; thread < thread_count; ++thread )
                {
                    const std::size_t k_begin = JobPointer(n,thread_count,thread  );
                    const std::size_t k_end   = JobPointer(n,thread_count,thread+1);
                    
                    for( std::size_t k = k_begin; k < k_end; ++k )
                    {
                        combine_scalars<alpha_flag,beta_flag>( alpha, x[k], beta, y[k] );
                    }
                }
            }
            else
            {
                for( std::size_t k = 0; k < n; ++k )
                {
                    combine_scalars<alpha_flag,beta_flag>( alpha, x[k], beta, y[k] );
                }
            }
        }
    }

    template<
        std::size_t n,
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        typename R_0, typename S_0, typename R_1, typename S_1
    >
    constexpr force_inline void combine_buffers(
        const R_0 & alpha, ptr<S_0> x,
        const R_1 & beta,  mut<S_1> y
    )
    {
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

        static_assert( IsComplex<S_1> || (IsReal<R_0> && IsReal<S_0> && IsReal<R_1>),
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
        
        if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Zero) )
        {
            zerofy_buffer<n>(y);
        }
        else if constexpr ( (beta_flag == Flag::Zero) && (alpha_flag == Flag::Plus) )
        {
            copy_buffer<n>(x,y);
        }
        else
        {
            for( std::size_t k = 0; k < n; ++k )
            {
                combine_scalars<alpha_flag,beta_flag>(alpha, x[k], beta, y[k]);
            }
        }
    }

} // namespace Tools
