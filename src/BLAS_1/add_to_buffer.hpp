#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline
    std::enable_if_t<
        std::is_same_v<R,S> || (ScalarTraits<S>::IsComplex && std::is_same_v<R,typename ScalarTraits<S>::Real>),
        void
    >
    add_to_buffer(
        const R * restrict const from,
              S * restrict const to,
        const size_t n
    )
    {
        for( size_t i = 0; i < n; ++i )
        {
            to[i] += from[i];
        }
    }
    
    template <size_t n, typename R, typename S>
    force_inline
    std::enable_if_t<
        std::is_same_v<R,S> || (ScalarTraits<S>::IsComplex && std::is_same_v<R,typename ScalarTraits<S>::Real>),
        void
    >
    add_to_buffer(
        const R * restrict const from,
              S * restrict const to
    )
    {
        for( size_t i = 0; i < n; ++i )
        {
            to[i] += from[i];
        }
    }
    
} // namespace Tools


