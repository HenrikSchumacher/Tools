#pragma once

namespace Tools
{
    enum class ScalarFlag : char
    {
        Generic = 2,
        Plus    = 1,
        Minus   = -1,
        Zero    = 0
    };
    
    template<
        ScalarFlag alpha_flag, ScalarFlag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline void combine_buffers( const R_0 alpha_, ptr<S_0> x, const R_1 beta_, mut<S_1> y, const size_t n )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == 0, then it assumes alpha = 0.
        // If alpha_flag == 1, then it assumes alpha = 1.
        
        // If beta_flag == 0, then it assumes beta = 0.
        // If beta_flag == 1, then it assumes beta = 1.
        
        // For all other values of alpha_flag and beta_flag, it assumes generic of alpha and beta (and hence performs the actual computation).
        
        const typename ScalarCast<R_0,S_0>::Type alpha = scalar_cast<S_0>(alpha_);
        const typename ScalarCast<R_1,S_1>::Type beta  = scalar_cast<S_0>(beta_ );
        
        if constexpr ( alpha_flag == ScalarFlag::Plus )
        {
            // alpha == 1;
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                copy_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                add_to_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(x[k]) - y[k];
                }
            }
            else
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(x[k]) + beta * y[k];
                }
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Minus )
        {
            // alpha == -1;
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                copy_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] -= scalar_cast<S_1>(x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = -y[k] - scalar_cast<S_1>(x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = beta * y[k] - scalar_cast<S_1>(x[k]);
                }
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Zero )
        {
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                zerofy_buffer(y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                // do nothing;
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = - y[k];
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                scale_buffer(beta,y,n);
            }
        }
        else
        {
            // alpha arbitrary;
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = alpha * x[k];
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] += scalar_cast<S_1>(alpha * x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(alpha * x[k]) - y[k];
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                // general alpha and general beta
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(alpha * x[k]) + (beta * y[k]);
                }
            }
        }
    }
    
    
    template<
        ScalarFlag alpha_flag, ScalarFlag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline void combine_buffers( const R_0 alpha_, ptr<S_0> x, const R_1 beta_, mut<S_1> y, const size_t n,
        const size_t thread_count
    )
    {
        // This routine computes y[i] = alpha * x[i] + beta * y[i].
        // Depending on the values of alpha_flag and beta_flag, it takes several short cuts:
        // If alpha_flag == 0, then it assumes alpha = 0.
        // If alpha_flag == 1, then it assumes alpha = 1.
        
        // If beta_flag == 0, then it assumes beta = 0.
        // If beta_flag == 1, then it assumes beta = 1.
        
        // For all other values of alpha_flag and beta_flag, it assumes generic of alpha and beta (and hence performs the actual computation).
        
        const typename ScalarCast<R_0,S_0>::Type alpha = scalar_cast<S_0>(alpha_);
        const typename ScalarCast<R_1,S_1>::Type beta  = scalar_cast<S_0>(beta_ );
        
        if( thread_count <= 1 )
        {
            combine_buffers<alpha_flag,beta_flag>(alpha,x,beta,y,n);
        }
        else
        {
            if constexpr ( alpha_flag == ScalarFlag::Plus )
            {
                // alpha == 1;
                if constexpr ( beta_flag == ScalarFlag::Zero )
                {
                    copy_buffer(x,y,n,thread_count);
                }
                else if constexpr ( beta_flag == ScalarFlag::Plus )
                {
                    add_to_buffer(x,y,n,thread_count);
                }
                else if constexpr ( beta_flag == ScalarFlag::Minus )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = scalar_cast<S_1>(x[k]) - y[k];
                        }
                    }
                }
                else
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = scalar_cast<S_1>(x[k]) + beta * y[k];
                        }
                    }
                }
            }
            else if constexpr ( alpha_flag == ScalarFlag::Minus )
            {
                // alpha == -1;
                if constexpr ( beta_flag == ScalarFlag::Zero )
                {
                    copy_buffer(x,y,n,thread_count);
                }
                else if constexpr ( beta_flag == ScalarFlag::Plus )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] -= scalar_cast<S_1>(x[k]);
                        }
                    }
                }
                else if constexpr ( beta_flag == ScalarFlag::Minus )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = -y[k] - scalar_cast<S_1>(x[k]);
                        }
                    }
                }
                else if constexpr ( beta_flag == ScalarFlag::Generic )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = beta * y[k] - scalar_cast<S_1>(x[k]);
                        }
                    }
                }
            }
            else if constexpr ( alpha_flag == ScalarFlag::Zero )
            {
                if constexpr ( beta_flag == ScalarFlag::Zero )
                {
                    zerofy_buffer(y,n,thread_count);
                }
                else if constexpr ( beta_flag == ScalarFlag::Plus )
                {
                    // do nothing;
                }
                else if constexpr ( beta_flag == ScalarFlag::Minus )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = - y[k];
                        }
                    }
                }
                else if constexpr ( beta_flag == ScalarFlag::Generic )
                {
                    scale_buffer(beta,y,n,thread_count);
                }
            }
            else
            {
                // alpha arbitrary;
                if constexpr ( beta_flag == ScalarFlag::Zero )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = scalar_cast<S_1>(alpha * x[k]);
                        }
                    }
                }
                else if constexpr ( beta_flag == ScalarFlag::Plus )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] += scalar_cast<S_1>(alpha * x[k]);
                        }
                    }
                }
                else if constexpr ( beta_flag == ScalarFlag::Minus )
                {
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = scalar_cast<S_1>(alpha * x[k]) - y[k];
                        }
                    }
                }
                else if constexpr ( beta_flag == ScalarFlag::Generic )
                {
                    // general alpha and general beta
                    #pragma omp parallel for num_threads( thread_count )
                    for( size_t thread = 0; thread < thread_count; ++thread )
                    {
                        const size_t k_begin = JobPointer(n,thread_count,thread  );
                        const size_t k_end   = JobPointer(n,thread_count,thread+1);
                        
                        for( size_t k = k_begin; k < k_end; ++k )
                        {
                            y[k] = scalar_cast<S_1>(alpha * x[k]) + (beta * y[k]);
                        }
                    }
                }
            }
        }
    }

    template<
        size_t n,
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
        
        // For all other values of alpha_flag and beta_flag, it assumes generic of alpha and beta (and hence performs the actual computation).
        
        const typename ScalarCast<R_0,S_0>::Type alpha = scalar_cast<S_0>(alpha_);
        const typename ScalarCast<R_1,S_1>::Type beta  = scalar_cast<S_0>(beta_ );
        
        if constexpr ( alpha_flag == ScalarFlag::Plus )
        {
            // alpha == 1;
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                copy_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                add_to_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(x[k]) - y[k];
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(x[k]) + beta * y[k];
                }
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Minus )
        {
            // alpha == -1;
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                copy_buffer(x,y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] -= scalar_cast<S_1>(x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = -y[k] - scalar_cast<S_1>(x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = beta * y[k] - scalar_cast<S_1>(x[k]);
                }
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Zero )
        {
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                zerofy_buffer(y,n);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                // do nothing;
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = - y[k];
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                scale_buffer(beta,y,n);
            }
        }
        else
        {
            // alpha arbitrary;
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(alpha * x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] += scalar_cast<S_1>(alpha * x[k]);
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(alpha * x[k]) - y[k];
                }
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                // general alpha and general beta
                for( size_t k = 0; k < n; ++k )
                {
                    y[k] = scalar_cast<S_1>(alpha * x[k]) + (beta * y[k]);
                }
            }
        }
    }

} // namespace Tools

