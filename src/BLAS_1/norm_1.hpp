#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename Scalar::Real<S> norm_1( cptr<S> z, const Size_T n )
    {
        typename Scalar::Real<S> norm = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            norm += Abs(z[i]);
        }
        return norm;
    }

    template <Size_T n, typename S>
    force_inline typename Scalar::Real<S> norm_1( cptr<S> z )
    {
        typename Scalar::Real<S> norm = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            norm += Abs(z[i]);
        }
        return norm;

    }
    
} // namespace Tools

