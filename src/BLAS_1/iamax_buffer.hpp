#pragma once

namespace Tools
{
    
    template <typename S>
    force_inline std::size_t iamax_buffer( ptr<S> z, const std::size_t n )
    {
        using R = typename Scalar::Real<S>;
        
        R max = 0;
        std::size_t pos = 0;
        
        for( std::size_t i = 0; i < n; ++i )
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

    template <std::size_t n, typename S>
    force_inline std::size_t iamax_buffer( ptr<S> z )
    {
        using R = typename Scalar::Real<S>;
        
        R max = 0;
        std::size_t pos = 0;
        
        for( std::size_t i = 0; i < n; ++i )
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
    force_inline std::size_t imax_buffer( ptr<S> x, const std::size_t n )
    {
        S max = std::numeric_limits<S>::lowest();
        std::size_t pos = 0;
        for( std::size_t i = 0; i < n; ++i )
        {
            if( x[i] > max )
            {
                pos = i;
                max = x[i];
            }
        }
        return pos;
    }

    template <std::size_t n, typename S>
    force_inline std::size_t imax_buffer( ptr<S> x )
    {
        S max = std::numeric_limits<S>::lowest();
        std::size_t pos = 0;
        for( std::size_t i = 0; i < n; ++i )
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
    force_inline std::size_t imin_buffer( ptr<S> x, const std::size_t n )
    {
        S min = std::numeric_limits<S>::max();
        std::size_t pos = 0;
        for( std::size_t i = 0; i < n; ++i )
        {
            if( x[i] < min )
            {
                pos = i;
                min = x[i];
            }
        }
        return pos;
    }

    template <std::size_t n, typename S>
    force_inline std::size_t imin_buffer( ptr<S> x )
    {
        S min = std::numeric_limits<S>::max();
        std::size_t pos = 0;
        for( std::size_t i = 0; i < n; ++i )
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

