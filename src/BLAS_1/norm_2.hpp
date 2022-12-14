#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename ScalarTraits<S>::Real norm_2( ptr<S> z, const size_t n )
    {
        typename ScalarTraits<S>::Real norm_squared = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            norm_squared += abs_squared(z[i]);
        }
        return std::sqrt(norm_squared);
    }

    template <size_t n, typename S>
    force_inline typename ScalarTraits<S>::Real norm_2( ptr<S> z )
    {
        typename ScalarTraits<S>::Real norm_squared = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            norm_squared += abs_squared(z[i]);
        }
        return std::sqrt(norm_squared);
    }
    
} // namespace Tools
