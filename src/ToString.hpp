#pragma once

namespace Tools
{
    template <typename T>
    std::string ToString(const T & a_value, const int p = 16)
    {
        std::stringstream sout;
        sout.precision(p);
        sout << a_value;
        return sout.str();
    }

    template <typename T>
    std::string ToString(const std::complex<T> & z, const int p = 16)
    {
        std::stringstream sout;
        sout.precision(p);
        sout << real(z) << ((imag(z) < static_cast<T>(0)) ? " + " : " - ") << std::abs(imag(z)) << " I" ;
        return sout.str();
    }
    
    template<typename T>
    std::string ToString( const std::vector<T> & a, const int p = 16)
    {
        std::stringstream sout;
        sout.precision(p);
        sout << "{ ";
        if( a.size() > 0 )
        {
            sout << a[0];
        }
        
        for( size_t i = 1; i < a.size(); ++i )
        {
            sout << ", " << a[i];
        }
        sout << " }";
        return sout.str();
    }
    
    template<typename T>
    std::string ToString( const T * const a, const size_t n, const int p = 16)
    {
        std::stringstream sout;
        sout.precision(p);
        sout << "{ ";
        if( n > 0 )
        {
            sout << a[0];
        }
        
        for( size_t i = 1; i < n; ++i )
        {
            sout << ", " << a[i];
        }
        sout << " }";
        return sout.str();
    }

} // namespace Tools
