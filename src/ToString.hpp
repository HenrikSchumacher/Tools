#pragma once

namespace Tools
{
    [[nodiscard]] constexpr std::string ToString( const std::string & s )
    {
        return s;
    }
    
    [[nodiscard]] constexpr std::string ToString( const char * ptr )
    {
        return std::string(ptr);
    }
    
    [[nodiscard]] constexpr std::string ToString( char * ptr )
    {
        return std::string(ptr);
    }
    
    [[nodiscard]] std::string ToString( const double & value )
    {
        return std::format("{: .17g}",value);
    }
    
    [[nodiscard]] std::string ToString( const float & value )
    {
        return std::format("{: .8g}",value);
    }
    
    [[nodiscard]] std::string ToString( const std::complex<double> & z )
    {
        return std::format("{: .17g} {:+.17g}I",std::real(z),std::imag(z));
    }
                           
    [[nodiscard]] std::string ToString( const std::complex<float> & z )
    {
        return std::format("{: .8g} {:+.8g}I",std::real(z),std::imag(z));
    }
    
    template<FloatQ T>
    [[nodiscard]] std::string ToMathematicaString( T x )
    {
        std::string s = ToString(x);
        
        Size_T p = s.find("e");
        
        if(p != std::string::npos)
        {
            s.replace(p, 1, "*^");
        }
        return s;
    }
    
    template<FloatQ T>
    std::ostream & operator<<( std::ostream & sout, const std::complex<T> & z )
    {
        return sout << ToString(z);
    }

    template<typename T>
    [[nodiscard]] std::string ToString( T * ptr )
    {
        return std::format( "{:p}", (void *)ptr );
    }
    
    template<typename T>
    [[nodiscard]] std::string ToString( T const * ptr )
    {
        return std::format( "{:p}", (void const *)ptr );
    }

    template<IntQ T>
    [[nodiscard]] constexpr std::string ToString( const T & value )
    {
        // TODO: We need the C++23 feature `if consteval` here.
//        if( std::is_constant_evaluated() )
//        {
//            return std::string(to_ct_string(value));
//        }
//        else
        {
            return std::format("{:d}",value);
        }
    }
    
#ifdef TOOLS_INT128_AVAILABLE
    // MSVC's STL has no std::formatter for __int128, so 128-bit integers get
    // their own overloads. These avoid 128-bit division, too: clang targeting
    // MSVC does not link compiler-rt by default, and that is where __udivti3
    // and __umodti3 live. So we split the value into four 32-bit limbs and do
    // long division by 10 in 64-bit arithmetic.
    
    [[nodiscard]] constexpr std::string ToString( const UInt128 & value )
    {
        std::uint64_t limbs [4] = {
            static_cast<std::uint64_t>(value >> 96) & 0xFFFFFFFFu,
            static_cast<std::uint64_t>(value >> 64) & 0xFFFFFFFFu,
            static_cast<std::uint64_t>(value >> 32) & 0xFFFFFFFFu,
            static_cast<std::uint64_t>(value      ) & 0xFFFFFFFFu
        };
        
        // 2^128 - 1 has 39 decimal digits.
        char digits [39] = {};
        Size_T pos = 39;
        
        do
        {
            std::uint64_t r = 0;
            
            for( std::uint64_t & limb : limbs )
            {
                const std::uint64_t x = (r << 32) | limb;
                limb = x / 10;
                r    = x % 10;
            }
            
            digits[--pos] = static_cast<char>('0' + r);
        }
        while( (limbs[0] | limbs[1] | limbs[2] | limbs[3]) != 0 );
        
        return std::string( &digits[pos], Size_T(39) - pos );
    }
    
    [[nodiscard]] constexpr std::string ToString( const Int128 & value )
    {
        // Negate in unsigned arithmetic, so that the minimum value works, too.
        if( value < Int128(0) )
        {
            return "-" + ToString( UInt128(0) - static_cast<UInt128>(value) );
        }
        else
        {
            return ToString( static_cast<UInt128>(value) );
        }
    }
#endif // TOOLS_INT128_AVAILABLE
    
    
//    template<typename T>
//    constexpr bool EnumClassQ = std::is_enum<T>::value && !std::is_convertible<T,int>::value;
    
    template<typename T>
    concept EnumClassQ = std::is_enum<T>::value && !std::is_convertible<T,int>::value;
    
    
    template <EnumClassQ T>
    [[nodiscard]] constexpr std::string ToString( const T & value )
    {
        return ToString( ToUnderlying(value) );
    }
    
    template <typename S, typename T>
    [[nodiscard]] constexpr std::string ToString( const std::pair<S,T> & p )
    {
        std::string s ("{ ");
        s += ToString(p.first);
        s += ", ";
        s += ToString(p.second);
        s += " }";
        return s;
    }
    
    // Some forward declarations. We can implement `ToString` only after we have `OutString`. And for `OutString` it would be nice to have things like `print` etc. for debugging and error handling.
    class OutString;
    
    template<typename T>
    [[nodiscard]] std::string ToString( cref<std::vector<T>> v );
    
    template<typename T, Size_T N>
    [[nodiscard]] std::string ToString( const std::array<T,N> & v );
    
    
    
    
    template<typename T>
    [[nodiscard]] std::string StringWithLeadingZeroes(
        const T val, const int width
    )
    {
        std::stringstream s;
        s << std::setw(width) << std::setfill('0') << val;
        return s.str();
    }

    [[nodiscard]] constexpr std::string BoolString( const bool b )
    {
        return b ? "True" : "False";
    }
    
    
} // namespace Tools
