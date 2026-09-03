#pragma once

namespace Tools
{

    namespace Details
    {
        template<Stringy S>
        TOOLS_FORCE_INLINE constexpr
        void StringCatHelper( std::string & str, S && s )
        {
            str.append(std::forward<S>(s));
        }
        
        template<Stringy S, Stringy... Args>
        TOOLS_FORCE_INLINE constexpr
        void StringCatHelper(
            std::string & str, S && s, Args &&... args
        )
        {
            str.append(std::forward<S>(s));
            StringCatHelper(str, std::forward<Args>(args)...);
        }
        
    } // namespace Details

    /*!@brief Meow!*/
    //    template<Stringy S>
    //    TOOLS_FORCE_INLINE constexpr std::string StringCat( const S & s )
    //    {
    //        return std::string(s);
    //    }

    template<Stringy S>
    TOOLS_FORCE_INLINE constexpr std::string StringCat( S && s )
    {
        return std::string(std::forward<S>(s));
    }

    /*!@brief Meow!*/
    TOOLS_FORCE_INLINE constexpr std::string StringCat( std::string && s )
    {
        return std::move(s);
    }


    /*!@brief Meow!*/
    template<Stringy... Args>
    TOOLS_FORCE_INLINE
    std::string StringCat( std::string && s, Args &&... args )
    {
        const Size_T size = s.size() + CharCount(std::forward<Args>(args)...);
        std::string str;
        if( s.size() < size )
        {
            str.reserve(size);
            str.append(s);
        }
        else
        {
            str = std::move(s);
        }

        Details::StringCatHelper(str, std::forward<Args>(args)...);
        return str;
    }

    /*!@brief Meow!*/
    template<Stringy S, Stringy... Args>
    TOOLS_FORCE_INLINE
    std::string StringCat( S && s, Args &&... args )
    {
        const Size_T size = CharCount(std::forward<S>(s))
                          + CharCount(std::forward<Args>(args)...);
        std::string str;
        str.reserve(size);
        str.append(std::forward<S>(s));
        Details::StringCatHelper(str, std::forward<Args>(args)...);
        return str;
    }
    
} // namespace Tools
