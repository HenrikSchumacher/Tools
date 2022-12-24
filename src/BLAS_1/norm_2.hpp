#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline ScalarTraits<S>::Real norm_2(
        const S * restrict const z,
        const size_t n
    )
    {
        ScalarTraits<S>::Real norm_squared = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            norm_squared += std::abs_squared(z[i]);
        }
        return std::sqrt(norm_squared);
    }

    template <size_t n, typename S>
    force_inline ScalarTraits<S>::Real norm_2(
        const S * restrict const z
    )
    {
        ScalarTraits<S>::Real norm_squared = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            norm_squared += std::abs_squared(z[i]);
        }
        return std::sqrt(norm_squared);
    }
    
} // namespace Tools
