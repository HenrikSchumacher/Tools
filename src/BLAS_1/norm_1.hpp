#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename Scalar::Real<S> norm_1( ptr<S> z, const std::size_t n )
    {
        typename Scalar::Real<S> norm = 0;
        
        for( std::size_t i = 0; i < n; ++i )
        {
            norm += std::abs(z[i]);
        }
        return norm;
    }

    template <std::size_t n, typename S>
    force_inline typename Scalar::Real<S> norm_1( ptr<S> z )
    {
        typename Scalar::Real<S> norm = 0;
        
        for( std::size_t i = 0; i < n; ++i )
        {
            norm += std::abs(z[i]);
        }
        return norm;

    }
    
} // namespace Tools

