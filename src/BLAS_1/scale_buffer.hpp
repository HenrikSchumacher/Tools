#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline
    std::enable_if_t<
        std::is_same_v<R,S>
        ||
        (ScalarTraits<S>::IsComplex && std::is_same_v<R,typename ScalarTraits<S>::Real>)
        ,
        void
    >
    scale_buffer( const R beta, mut<S> y, const size_t n )
    {
        for( size_t i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }
    
    template <typename R, typename S>
    force_inline
    std::enable_if_t<
        std::is_same_v<R,S>
        ||
        (ScalarTraits<S>::IsComplex && std::is_same_v<R,typename ScalarTraits<S>::Real>)
        ,
        void
    >
    scale_buffer( const R beta, mut<S> y, const size_t n, const size_t thread_count )
    {
        if( thread_count <= 1 )
        {
            scale_buffer( beta, y, n );
        }
        else
        {
            #pragma omp parallel for num_threads( thread_count )
            for( size_t thread = 0; thread < thread_count; ++thread )
            {
                const size_t i_begin = JobPointer(n,thread_count,thread  );
                const size_t i_end   = JobPointer(n,thread_count,thread+1);
                
                for( size_t i = i_begin; i < i_end; ++i )
                {
                    y[i] *= beta;
                }
            }
        }
    }

    template <size_t n, typename R, typename S>
    force_inline
    std::enable_if_t<
        std::is_same_v<R,S>
        ||
        (ScalarTraits<S>::IsComplex && std::is_same_v<R,typename ScalarTraits<S>::Real>)
        ,
        void
    >
    scale_buffer( const R beta, mut<S> y )
    {
        for( size_t i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }

    
} // namespace Tools
