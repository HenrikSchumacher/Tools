#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline size_t iamax_buffer( ptr<S> z, const size_t n )
    {
        using R = typename ScalarTraits<S>::Real;
        
        R max = 0;
        size_t pos = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            R abs_z = abs_squared(z[i]);
            if( abs_z > max )
            {
                pos = i;
                max = abs_z;
            }
        }
        return pos;
    }

    template <size_t n, typename S>
    force_inline size_t iamax_buffer( ptr<S> z )
    {
        using R = typename ScalarTraits<S>::Real;
        
        R max = 0;
        size_t pos = 0;
        
        for( size_t i = 0; i < n; ++i )
        {
            R abs_z = abs_squared(z[i]);
            if( abs_z > max )
            {
                pos = i;
                max = abs_z;
            }
        }
        return pos;
    }


    template <typename S>
    force_inline size_t imax_buffer( ptr<S> x, const size_t n )
    {
        S max = std::numeric_limits<S>::lowest();
        size_t pos = 0;
        for( size_t i = 0; i < n; ++i )
        {
            if( x[i] > max )
            {
                pos = i;
                max = x[i];
            }
        }
        return pos;
    }

    template <size_t n, typename S>
    force_inline size_t imax_buffer( ptr<S> x )
    {
        S max = std::numeric_limits<S>::lowest();
        size_t pos = 0;
        for( size_t i = 0; i < n; ++i )
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
    force_inline size_t imin_buffer( ptr<S> x, const size_t n )
    {
        S min = std::numeric_limits<S>::max();
        size_t pos = 0;
        for( size_t i = 0; i < n; ++i )
        {
            if( x[i] < min )
            {
                pos = i;
                min = x[i];
            }
        }
        return pos;
    }

    template <size_t n, typename S>
    force_inline size_t imin_buffer( ptr<S> x )
    {
        S min = std::numeric_limits<S>::max();
        size_t pos = 0;
        for( size_t i = 0; i < n; ++i )
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

