#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename Scalar::Real<S> norm_2( cptr<S> z, const Size_T n )
    {
        typename Scalar::Real<S> norm_squared = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            norm_squared += AbsSquared(z[i]);
        }
        return Sqrt(norm_squared);
    }

    template <Size_T n, typename S>
    force_inline typename Scalar::Real<S> norm_2( cptr<S> z )
    {
        typename Scalar::Real<S> norm_squared = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            norm_squared += AbsSquared(z[i]);
        }
        return Sqrt(norm_squared);
    }
    
} // namespace Tools
