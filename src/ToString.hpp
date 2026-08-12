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
        if( std::is_constant_evaluated() )
        {
            return std::string(to_ct_string(value));
        }
        else
        {
            return std::format("{:d}",value);
        }
    }
    
    
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
    [[nodiscard]] OutString ToString( cref<std::vector<T>> v );
    
    template<typename T, Size_T N>
    [[nodiscard]] OutString ToString( const std::array<T,N> & v );
    
    
    
    
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
