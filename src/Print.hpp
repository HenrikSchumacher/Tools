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
    
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
    inline void print( const std::string & s )
    {
        #pragma omp critical(cout)
        {
            mma::print( std::string( 2 * (Timers::time_stack.size()), ' ') + s );
        }
//        mma::print( s );
    }
#else
    inline void print( const std::string & s )
    {
        #pragma omp critical(cout)
        {
            std::cout << s << std::endl;
        }
    }
#endif
    
    template<typename T>
    inline void valprint( const std::string & s, const T & value)
    {
        using namespace std;
        print( s + " = " + to_string(value) );
    }

    template<typename T>
    inline void valprint( const std::string & s, const T & value, const int p)
    {
        print( s + " = " + ToString(value, p) );
    }
    
    inline void valprint( const std::string & s, const std::string & value)
    {
        using namespace std;
        print( s + " = " + value );
    }
    
#define dump(x) valprint( std::string(#x), x );

} // namespace Tools
