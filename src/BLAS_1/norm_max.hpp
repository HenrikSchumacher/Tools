#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline typename Scalar::Real<S> norm_max( cptr<S> z, const Size_T n )
    {
        typename Scalar::Real<S> max = 0;
        
        if constexpr ( Scalar::RealQ<S> )
        {
            for( Size_T i = 0; i < n; ++i )
            {
                max = Max( max, Abs(z[i]) );
            }
            return max;
        }
        else
        {
            for( Size_T i = 0; i < n; ++i )
            {
                max = Max( max, AbsSquared(z[i]) );
            }
            return Sqrt(max);
        }
    }

    template <Size_T n, typename S>
    force_inline typename Scalar::Real<S> norm_max( cptr<S> z )
    {
        typename Scalar::Real<S> max = 0;
        
        if constexpr ( Scalar::RealQ<S> )
        {
            for( Size_T i = 0; i < n; ++i )
            {
                max = Max( max, Abs(z[i]) );
            }
            return max;
        }
        else
        {
            for( Size_T i = 0; i < n; ++i )
            {
                max = Max( max, AbsSquared(z[i]) );
            }
            return Sqrt(max);
        }
    }

    
} // namespace Tools


