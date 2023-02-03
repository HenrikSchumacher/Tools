#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename ScalarTraits<S>::Real norm_max( ptr<S> z, const std::size_t n )
    {
        typename ScalarTraits<S>::Real max = 0;
        
        for( std::size_t i = 0; i < n; ++i )
        {
            max = std::max( max, abs_squared(z[i]));
        }
        return std::sqrt(max);
    }

    template <std::size_t n, typename S>
    force_inline typename ScalarTraits<S>::Real norm_max( ptr<S> z )
    {
        typename ScalarTraits<S>::Real max = 0;
        
        for( std::size_t i = 0; i < n; ++i )
        {
            max = std::max( max, abs_squared(z[i]));
        }
        return std::sqrt(max);
    }

    
} // namespace Tools


