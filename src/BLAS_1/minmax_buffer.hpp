#pragma once

namespace Tools
{

    template <typename S>
    force_inline std::pair<S,S> minmax_buffer( ptr<S> z, const size_t n )
    {
        S min_ = std::numeric_limits<S>::max();
        S max_ = std::numeric_limits<S>::lowest();
        
        for( size_t i = 0; i < n; ++i )
        {
            min_ = std::min( min_, z[i] );
            max_ = std::max( max_, z[i] );
        }
        return std::pair<S,S>(min_,max_);
    }
    
    template <size_t n, typename S>
    force_inline std::pair<S,S> minmax_buffer( ptr<S> z )
    {
        S min_ = std::numeric_limits<S>::max();
        S max_ = std::numeric_limits<S>::lowest();
        
        for( size_t i = 0; i < n; ++i )
        {
            min_ = std::min( min_, z[i] );
            max_ = std::max( max_, z[i] );
        }
        return std::pair<S,S>(min_,max_);
    }
    
    
    
    template <typename S>
    force_inline S max_buffer( ptr<S> z, const size_t n )
    {
        S m = std::numeric_limits<S>::lowest();
        
        for( size_t i = 0; i < n; ++i )
        {
            m = std::max( m, z[i] );
        }
        return m;
    }
    
    template <size_t n, typename S>
    force_inline S max_buffer( ptr<S> z )
    {
        S m = std::numeric_limits<S>::lowest();
        
        for( size_t i = 0; i < n; ++i )
        {
            m = std::max( m, z[i] );
        }
        return m;
    }
    
    
    template <typename S>
    force_inline S min_buffer( ptr<S> z, const size_t n )
    {
        S m = std::numeric_limits<S>::max();
        
        for( size_t i = 0; i < n; ++i )
        {
            m = std::min( m, z[i] );
        }
        return m;
    }
    
    template <size_t n, typename S>
    force_inline S min_buffer( ptr<S> z )
    {
        S m = std::numeric_limits<S>::max();
        
        for( size_t i = 0; i < n; ++i )
        {
            m = std::min( m, z[i] );
        }
        return m;
    }
    
}
