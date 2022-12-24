#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline ScalarTraits<S>::Real norm_1(
        const S * restrict const z,
        const size_t n
    )
    {
        ScalarTraits<S>::Real norm = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            norm += std::abs(z[i]);
        }
        return norm;
    }

    template <size_t n, typename S>
    force_inline ScalarTraits<S>::Real norm_1(
        const S * restrict const z
    )
    {
        ScalarTraits<S>::Real norm = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            norm += std::abs(z[i]);
        }
        return norm;

    }
    
} // namespace Tools

