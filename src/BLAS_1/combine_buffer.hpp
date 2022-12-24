#pragma once

namespace Tools
{
    
    template<
        int alpha_flag, int beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline
    std::enable_if_t<
        (
            std::is_same_v<R_0,S_0>
            ||
            (ScalarTraits<S_0>::IsComplex && std::is_same_v<R_0,typename ScalarTraits<S_0>::Real>)
        )
        &&
        (
            std::is_same_v<R_1,S_1>
            ||
            (ScalarTraits<S_1>::IsComplex && std::is_same_v<R_1,typename ScalarTraits<S_1>::Real>)
        )
        ,
        void
    >
    combine_buffers(
        const R_0                  alpha,
        const S_0 * restrict const x,
        const R_1                  beta,
              S_0 * restrict const y,
        const size_t               n
    )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == 0, then it assumes alpha = 0.
        // If alpha_flag == 1, then it assumes alpha = 1.
        
        // If beta_flag == 0, then it assumes beta = 0.
        // If beta_flag == 1, then it assumes beta = 1.
        
        // For all other values of alpha_flag and beta_flag, it assumes generic of alpha and beta (and hence performs the actual computation).
        
        if constexpr ( alpha_flag == 1 )
        {
            // alpha == 1;
            if constexpr ( beta_flag == 0 )
            {
                copy_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == 1 )
            {
                add_to_buffer(x,y,n);
            }
            else
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = static_cast<S_1>(x[k]) + beta * y[k];
                }
            }
        }
        else if constexpr ( alpha_flag == 0 )
        {
            if constexpr ( beta_flag == 0 )
            {
                zerofy_buffer(y,n);
            }
            else if constexpr ( beta_flag == 1 )
            {
                // do nothing;
            }
            else
            {
                scale_buffer(beta,y,n);
            }
        }
        else
        {
            // alpha arbitrary;
            if constexpr ( beta_flag == 0 )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = static_cast<S_1>(alpha * x[k]);
                }
            }
            else if constexpr ( beta_flag == 1 )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] += static_cast<S_1>(alpha * x[k]);
                }
            }
            else
            {
                // general alpha and general beta
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = static_cast<S_1>(alpha * x[k]) + (beta * y[k]);
                }
            }
        }
    }

    template<
        size_t n,
        int alpha_flag, int beta_flag,
        typename R_0, typename S_0, typename R_1, typename S_1
    >
    force_inline
    std::enable_if_t<
        (
            std::is_same_v<R_0,S_0>
            ||
            (ScalarTraits<S_0>::IsComplex && std::is_same_v<R_0,typename ScalarTraits<S_0>::Real>)
        )
        &&
        (
            std::is_same_v<R_1,S_1>
            ||
            (ScalarTraits<S_1>::IsComplex && std::is_same_v<R_1,typename ScalarTraits<S_1>::Real>)
        )
        ,
        void
    >
    combine_buffers(
        const R_0                  alpha,
        const S_0 * restrict const x,
        const R_1                  beta,
              S_0 * restrict const y
    )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == 0, then it assumes alpha = 0.
        // If alpha_flag == 1, then it assumes alpha = 1.
        
        // If beta_flag == 0, then it assumes beta = 0.
        // If beta_flag == 1, then it assumes beta = 1.
        
        // For all other values of alpha_flag and beta_flag, it assumes generic of alpha and beta (and hence performs the actual computation).
        
        if constexpr ( alpha_flag == 1 )
        {
            // alpha == 1;
            if constexpr ( beta_flag == 0 )
            {
                copy_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == 1 )
            {
                add_to_buffer(x,y,n);
            }
            else
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = static_cast<S_1>(x[k]) + beta * y[k];
                }
            }
        }
        else if constexpr ( alpha_flag == 0 )
        {
            if constexpr ( beta_flag == 0 )
            {
                zerofy_buffer(y,n);
            }
            else if constexpr ( beta_flag == 1 )
            {
                // do nothing;
            }
            else
            {
                scale_buffer(beta,y,n);
            }
        }
        else
        {
            // alpha arbitrary;
            if constexpr ( beta_flag == 0 )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = static_cast<S_1>(alpha * x[k]);
                }
            }
            else if constexpr ( beta_flag == 1 )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] += static_cast<S_1>(alpha * x[k]);
                }
            }
            else
            {
                // general alpha and general beta
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = static_cast<S_1>(alpha * x[k]) + (beta * y[k]);
                }
            }
        }
    }

} // namespace Tools

