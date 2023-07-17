#pragma once

namespace Tools
{

    template <typename S>
    force_inline std::pair<S,S> minmax_buffer( cptr<S> z, const Size_T n )
    {
        S min_ = std::numeric_limits<S>::max();
        S max_ = std::numeric_limits<S>::lowest();
        
        for( Size_T i = 0; i < n; ++i )
        {
            min_ = std::min( min_, z[i] );
            max_ = std::max( max_, z[i] );
        }
        return std::pair<S,S>(min_,max_);
    }
    
    template <Size_T n, typename S>
    force_inline std::pair<S,S> minmax_buffer( cptr<S> z )
    {
        S min_ = std::numeric_limits<S>::max();
        S max_ = std::numeric_limits<S>::lowest();
        
        for( Size_T i = 0; i < n; ++i )
        {
            min_ = std::min( min_, z[i] );
            max_ = std::max( max_, z[i] );
        }
        return std::pair<S,S>(min_,max_);
    }
    
    
    
    template <typename S>
    force_inline S max_buffer( cptr<S> z, const Size_T n )
    {
        S m = std::numeric_limits<S>::lowest();
        
        for( Size_T i = 0; i < n; ++i )
        {
            m = std::max( m, z[i] );
        }
        return m;
    }
    
    template <Size_T n, typename S>
    force_inline S max_buffer( cptr<S> z )
    {
        S m = std::numeric_limits<S>::lowest();
        
        for( Size_T i = 0; i < n; ++i )
        {
            m = std::max( m, z[i] );
        }
        return m;
    }
    
    
    template <typename S>
    force_inline S min_buffer( cptr<S> z, const Size_T n )
    {
        S m = std::numeric_limits<S>::max();
        
        for( Size_T i = 0; i < n; ++i )
        {
            m = std::min( m, z[i] );
        }
        return m;
    }
    
    template <Size_T n, typename S>
    force_inline S min_buffer( cptr<S> z )
    {
        S m = std::numeric_limits<S>::max();
        
        for( Size_T i = 0; i < n; ++i )
        {
            m = std::min( m, z[i] );
        }
        return m;
    }
    
}
