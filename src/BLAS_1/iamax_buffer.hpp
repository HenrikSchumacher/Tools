#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline Size_T iamax_buffer( ptr<S> z, const Size_T n )
    {
        using R = typename Scalar::Real<S>;
        
        R max = 0;
        Size_T pos = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            R abs_z = Scalar::AbsSquared(z[i]);
            if( abs_z > max )
            {
                pos = i;
                max = abs_z;
            }
        }
        return pos;
    }

    template <Size_T n, typename S>
    force_inline Size_T iamax_buffer( ptr<S> z )
    {
        using R = typename Scalar::Real<S>;
        
        R max = 0;
        Size_T pos = 0;
        
        for( Size_T i = 0; i < n; ++i )
        {
            R abs_z = Scalar::AbsSquared(z[i]);
            if( abs_z > max )
            {
                pos = i;
                max = abs_z;
            }
        }
        return pos;
    }


    template <typename S>
    force_inline Size_T imax_buffer( ptr<S> x, const Size_T n )
    {
        S max = std::numeric_limits<S>::lowest();
        Size_T pos = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            if( x[i] > max )
            {
                pos = i;
                max = x[i];
            }
        }
        return pos;
    }

    template <Size_T n, typename S>
    force_inline Size_T imax_buffer( ptr<S> x )
    {
        S max = std::numeric_limits<S>::lowest();
        Size_T pos = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            if( x[i] > max )
            {
                pos = i;
                max = x[i];
            }
        }
        return pos;
    }

    
    template <typename S>
    force_inline Size_T imin_buffer( ptr<S> x, const Size_T n )
    {
        S min = std::numeric_limits<S>::max();
        Size_T pos = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            if( x[i] < min )
            {
                pos = i;
                min = x[i];
            }
        }
        return pos;
    }

    template <Size_T n, typename S>
    force_inline Size_T imin_buffer( ptr<S> x )
    {
        S min = std::numeric_limits<S>::max();
        Size_T pos = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            if( x[i] < min )
            {
                pos = i;
                min = x[i];
            }
        }
        return pos;
    }

    
} // namespace Tools

