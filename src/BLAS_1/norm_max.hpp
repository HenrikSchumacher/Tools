#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline ScalarTraits<S>::Real norm_max(
        const S * restrict const z,
        const size_t n
    )
    {
        ScalarTraits<S>::Real max = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            max = std::max( max, std::abs_squared(z[i]));
        }
        return std::sqrt(max);
    }

    template <size_t n, typename S>
    force_inline ScalarTraits<S>::Real norm_max(
        const S * restrict const z
    )
    {
        ScalarTraits<S>::Real max = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            max = std::max( max, std::abs_squared(z[i]));
        }
        return std::sqrt(max);
    }

    
} // namespace Tools


