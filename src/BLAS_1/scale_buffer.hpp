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
    scale_buffer(
        const R                  beta,
              S * restrict const y,
        const size_t n )
    {
        for( size_t i = 0; i < n; ++i )
        {
            y[i] *= beta;
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
    scale_buffer(
        const R                  beta,
              S * restrict const y
    )
    {
        for( size_t i = 0; i < n; ++i )
        {
            y[i] *= beta;
        }
    }

    
} // namespace Tools
