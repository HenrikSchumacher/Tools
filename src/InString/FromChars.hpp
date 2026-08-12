#pragma once

namespace Tools
{
    struct FromCharResult
    {
        const char * ptr = nullptr;
        bool failedQ = false;
    };
    
    template<typename T>
    struct FromChars
    {
        static constexpr bool implementedQ = false;
    };
    
    // Fix for older versions from https://www.cppstories.com/2019/07/detect-overload-from-chars/
    
    // In herit from false_type per default.
    template <typename T, typename = void>
    struct is_from_chars_convertible : std::false_type {};
    
    // SFINAE: if lookup is successful, inherit from true_type.
    template <typename T>
    struct is_from_chars_convertible<
        T,
        std::void_t<decltype(
            std::from_chars(
                std::declval<const char*>(),
                std::declval<const char*>(),
                std::declval<T&>()
            )
        )>
    > : std::true_type {};
    
    // Making requests less awkward
    template< class T> inline constexpr bool from_chars_availableQ = is_from_chars_convertible<T>::value;
    

    template<IntQ T>
    struct FromChars<T>
    {
        static constexpr bool implementedQ = true;
        
        FromCharResult operator()( const char * const begin, const char * const end, T & x ) const
        {
            auto r = std::from_chars( begin, end, x, 10 );
            return FromCharResult{ .ptr = r.ptr, .failedQ = (r.ec != std::errc{})};
        }
    };
    
    template<std::floating_point T>
    struct FromChars<T>
    {
        static constexpr bool implementedQ = true;
        
        FromCharResult operator()( const char * const begin, const char * const end, T & x ) const
        {
            if constexpr ( from_chars_availableQ<T> )
            {
                auto r = std::from_chars( begin, end, x, std::chars_format::general );
                return FromCharResult{.ptr = r.ptr, .failedQ = (r.ec != std::errc{})};
            }
            else
            {
                // A nasty, nasty work-around for some compilers that have not implemented std::from_chars for floating-point types.
                
                auto valid_charQ = []( const char c )
                {
                    return std::isalnum(c) || (c == '.') || (c == '-') || (c == '+');
                };
                
                if( (begin == end) || (*begin == '\0') )
                {
                    return FromCharResult{.ptr = begin, .failedQ = true};
                }
                
                // Find first non-whitespace character.
                const char * b = begin;
                while( std::isspace(*b) )
                {
                    ++b;
                    
                    if( (b == end) || (*b == '\0') )
                    {
                        return FromCharResult{.ptr = b, .failedQ = true};
                    }
                }

                // Find first character that cannot be part of floating-point number.
                const char * e = b;
                while( valid_charQ(*e) && (e < end) ) { ++e; }
                
                // Create a new std::string that internally creates a zero-terminated string. Then apply std::stod.
                std::string s (b, e);
                T value = 0;
                Size_T length = 0;
                bool failedQ = false;
                
                try
                {
                    if constexpr (std::is_same_v<T,float>)
                    {
                        value = std::stof(s, &length);
                    }
                    else if constexpr (std::is_same_v<T,double>)
                    {
                        value = std::stod(s, &length);
                    }
                    else if constexpr (std::is_same_v<T,long double>)
                    {
                        value = std::stold(s, &length);
                    }
                    else
                    {
                        failedQ = true;
                    }
                }
                catch (...)
                {
                    failedQ = true;
                }
                
                // We change the value of x only if no issues occurred, to emulate the behavior of std::from_chars.
                if( !failedQ ) { x = value; }
                
                return FromCharResult{.ptr = &begin[length], .failedQ = failedQ};
            }
        }
    };

    template<> struct FromChars<bool>
    {
        static constexpr bool implementedQ = true;
        
        FromCharResult operator()( const char * const begin, const char * const end, bool & x ) const
        {
            if( begin >= end )
            {
                return FromCharResult{ .ptr = begin, .failedQ = true };
            }
            
            // We take this very literal. Nothing except `0` or `1` is allowed here.
            if( *begin == '1' )
            {
                x = true;
            }
            else if( *begin == '0' )
            {
                x = false;
            }
            else
            {
                return FromCharResult{ .ptr = begin, .failedQ = true };
            }
            return FromCharResult{ .ptr = begin + 1, .failedQ = false };
        }
    };
    
    template<EnumClassQ X>
    struct FromChars<X>
    {
        // TODO: This just converts integers to the enum and does not test whether the read value is really in a state set by the enum class! This is a bit risky. Implementing such a check is a pain in the neck, though, as C++20 does not have introspection.
        
        using U = std::underlying_type_t<X>;
        static_assert(IntQ<U>, "");
        
        static constexpr bool implementedQ = true;
        
        FromCharResult operator()( const char * const begin, const char * const end, X & x ) const
        {
            return FromChars<U>()( begin, end, reinterpret_cast<U &>(x) );
        }
    };
    
} // namespace Tools
