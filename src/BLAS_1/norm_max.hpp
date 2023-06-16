#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename Scalar::Real<S> norm_max( ptr<S> z, const Size_T n )
    {
        typename Scalar::Real<S> max = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            max = std::max( max, Scalar::AbsSquared(z[i]));
        }
        return std::sqrt(max);
    }

    template <Size_T n, typename S>
    force_inline typename Scalar::Real<S> norm_max( ptr<S> z )
    {
        typename Scalar::Real<S> max = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            max = std::max( max, Scalar::AbsSquared(z[i]));
        }
        return std::sqrt(max);
    }

    
} // namespace Tools


