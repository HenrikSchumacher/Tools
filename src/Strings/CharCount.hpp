#pragma once

namespace Tools
{
    template<Size_T n>
    constexpr Size_T CharCount( const char(&c)[n] )
    {
        (void)c;
        return n-1;
    }

    Size_T CharCount( std::string_view s )
    {
        return s.size();
    }
    
    template<typename... S>
    Size_T CharCount( std::string_view s, S... args )
    {
        return s.size() + CharCount(args...);
    }
    
} // namespace Tools
