#pragma once

namespace Tools
{
    template<
        ScalarFlag alpha_flag, ScalarFlag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline void combine_buffers( const R_0 alpha_, ptr<S_0> x, const R_1 beta_, mut<S_1> y, const std::size_t n )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == ScalarFlag::Zero,  then it assumes alpha = 0.
        // If alpha_flag == ScalarFlag::Plus,  then it assumes alpha = 1.
        // If alpha_flag == ScalarFlag::Minus, then it assumes alpha = -1.
        // If alpha_flag == ScalarFlag::Generic, then it assumes generic values for alpha.
        
        // If beta_flag == ScalarFlag::Zero,  then it assumes beta = 0.
        // If beta_flag == ScalarFlag::Plus,  then it assumes beta = 1.
        // If beta_flag == ScalarFlag::Minus, then it assumes beta = -1.
        // If beta_flag == ScalarFlag::Generic, then it assumes generic values for beta.
        
        // Caution: The return type is _not_ guaranteed to be S_1 -- but it has the same precision.
        const auto alpha = scalar_cast<S_1>(alpha_);
        const auto beta  = scalar_cast<S_1>(beta_ );
        
        if constexpr ( (beta_flag == ScalarFlag::Zero) && (alpha_flag == ScalarFlag::Zero) )
        {
            zerofy_buffer(y,n);
        }
        else if constexpr ( (beta_flag == ScalarFlag::Zero) && (alpha_flag == ScalarFlag::Plus) )
        {
            copy_buffer(x,y,n);
        }
        else
        {
            for( std::size_t k = 0; k < n; ++k )
            {
                combine_scalars<alpha_flag,beta_flag>(alpha, x[k], beta, y[k]);
            }
        }
    }
    
    
    template<
        ScalarFlag alpha_flag, ScalarFlag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline void combine_buffers( const R_0 alpha_, ptr<S_0> x, const R_1 beta_, mut<S_1> y, const std::size_t n,
        const std::size_t thread_count
    )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == ScalarFlag::Zero,  then it assumes alpha = 0.
        // If alpha_flag == ScalarFlag::Plus,  then it assumes alpha = 1.
        // If alpha_flag == ScalarFlag::Minus, then it assumes alpha = -1.
        // If alpha_flag == ScalarFlag::Generic, then it assumes generic values for alpha.
        
        // If beta_flag == ScalarFlag::Zero,  then it assumes beta = 0.
        // If beta_flag == ScalarFlag::Plus,  then it assumes beta = 1.
        // If beta_flag == ScalarFlag::Minus, then it assumes beta = -1.
        // If beta_flag == ScalarFlag::Generic, then it assumes generic values for beta.
        
        // Caution: The return type is _not_ guaranteed to be S_1 -- but it has the same precision.
        const auto alpha = scalar_cast<S_1>(alpha_);
        const auto beta  = scalar_cast<S_1>(beta_ );
        
        if constexpr ( (beta_flag == ScalarFlag::Zero) && (alpha_flag == ScalarFlag::Zero) )
        {
            zerofy_buffer(y,n,thread_count);
        }
        else if constexpr ( (beta_flag == ScalarFlag::Zero) && (alpha_flag == ScalarFlag::Plus) )
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
                        combine_scalars<alpha_flag,beta_flag>(alpha, x[k], beta, y[k]);
                    }
                }
            }
            else
            {
                for( std::size_t k = 0; k < n; ++k )
                {
                    combine_scalars<alpha_flag,beta_flag>(alpha, x[k], beta, y[k]);
                }
            }
        }
    }

    template<
        std::size_t n,
        ScalarFlag alpha_flag, ScalarFlag beta_flag,
        typename R_0, typename S_0, typename R_1, typename S_1
    >
    force_inline void combine_buffers( const R_0 alpha_, ptr<S_0> x, const R_1 beta_, mut<S_1> y )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == ScalarFlag::Zero,  then it assumes alpha = 0.
        // If alpha_flag == ScalarFlag::Plus,  then it assumes alpha = 1.
        // If alpha_flag == ScalarFlag::Minus, then it assumes alpha = -1.
        // If alpha_flag == ScalarFlag::Generic, then it assumes generic values for alpha.
        
        // If beta_flag == ScalarFlag::Zero,  then it assumes beta = 0.
        // If beta_flag == ScalarFlag::Plus,  then it assumes beta = 1.
        // If beta_flag == ScalarFlag::Minus, then it assumes beta = -1.
        // If beta_flag == ScalarFlag::Generic, then it assumes generic values for beta.

        // Caution: The return type is _not_ guaranteed to be S_1 -- but it has the same precision.
        const auto alpha = scalar_cast<S_1>(alpha_);
        const auto beta  = scalar_cast<S_1>(beta_ );
        
        if constexpr ( (beta_flag == ScalarFlag::Zero) && (alpha_flag == ScalarFlag::Zero) )
        {
            zerofy_buffer<n>(y);
        }
        else if constexpr ( (beta_flag == ScalarFlag::Zero) && (alpha_flag == ScalarFlag::Plus) )
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

