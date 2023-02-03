#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename ScalarTraits<S>::Real norm_1( ptr<S> z, const std::size_t n )
    {
        typename ScalarTraits<S>::Real norm = 0;
        
        for( std::size_t i = 0; i < n; ++i )
        {
            norm += std::abs(z[i]);
        }
        return norm;
    }

    template <std::size_t n, typename S>
    force_inline typename ScalarTraits<S>::Real norm_1( ptr<S> z )
    {
        typename ScalarTraits<S>::Real norm = 0;
        
        for( std::size_t i = 0; i < n; ++i )
        {
            norm += std::abs(z[i]);
        }
        return norm;

    }
    
} // namespace Tools

