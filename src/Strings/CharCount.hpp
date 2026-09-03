#pragma once

namespace Tools
{
    template<Size_T n>
    constexpr Size_T CharCount( const char(&c)[n] )
    {
        (void)c;
        return n-1;
    }

    Size_T CharCount( const std::string & s )
    {
        return s.size();
    }
    
    Size_T CharCount( const std::string_view & s )
    {
        return s.size();
    }
    
    template<typename T>
    std::enable_if_t<ToChars<T>::implementedQ,Size_T>
    CharCount( const T & s )
    {
        (void)s;
        return ToChars<T>::char_count;
    }
    
    template<Stringy S, typename... Args>
    Size_T CharCount( const S & s, const Args &... args );
    template<NonStringy S, typename... Args>
    Size_T CharCount( const S & s, const Args &... args );
    
    template<Stringy S, typename... Args>
    Size_T CharCount( const S & s, const Args &... args )
    {
        using Tools::CharCount;
        return CharCount(std::string_view(s)) + CharCount(args...);
    }
    
    template<NonStringy S, typename... Args>
//    std::enable_if_t<ToChars<S>::implementedQ,Size_T>
    Size_T CharCount( const S & s, const Args &... args )
    {
        using Tools::CharCount;
        return CharCount(s) + CharCount(args...);
    }
    

    
} // namespace Tools
