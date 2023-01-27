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
        sout << real(z) << ((imag(z) >= static_cast<T>(0)) ? " + " : " - ") << std::abs(imag(z)) << " I" ;
        return sout.str();
    }
    
    template<typename T>
    struct Array1DToString
    {
        Array1DToString() = default;
        
        Array1DToString( int prec_ )
        :   prec( prec_ )
        {}
        
        ~Array1DToString() = default;
        
        std::string header = "";
        
        std::string vector_prefix = "{ ";
        
            std::string sep = ", ";
        
        std::string vector_suffix = "\n}";
        
        std::string footer = "";
        
        int prec = 16;
        
        std::string operator()( const T * const a, const size_t n ) const
        {
            std::stringstream sout;

            sout.precision(prec);

            sout << header;

            sout << vector_prefix;
            {
                if( n > 0 )
                {
                    sout << a[0];
                }

                for( size_t i = 1; i < n; ++i )
                {
                    sout << sep << a[i];
                }
            }
            sout << vector_suffix;

            sout << footer;

            return sout.str();
        }
        
    }; // Array1DToString

    template<typename T>
    struct Array2DToString
    {
        Array2DToString() = default;
        
        explicit Array2DToString( int prec )
        :   row_converter( prec )
        {}
        
        ~Array2DToString() = default;
        
        std::string header = "";
        
        std::string matrix_prefix = "{\n";
        
            std::string row_prefix = "\t";
        
            std::string row_suffix = "";
        
            std::string row_sep = ",\n";
        
        std::string matrix_suffix = "\n}";
        
        std::string footer = "";
        
        Array1DToString<T> row_converter;
        
        std::string operator()( const T * const a, const size_t rows, const size_t cols ) const
        {
            std::stringstream sout;

            sout << header;

            sout << matrix_prefix;
            {
                sout << row_prefix;

                if( rows > 0 )
                {
                    // write first row
                    sout << row_converter( a, cols );

                    // write remaining rows

                    for( size_t i = 1; i < rows; ++i )
                    {
                        sout << row_suffix << row_sep << row_prefix;

                        sout << row_converter( &a[cols*i], cols );
                    }
                }

                sout << row_suffix;
            }

            sout << matrix_suffix;

            sout << footer;

            return sout.str();
        }
        
    }; // Array2DToString

    
    
    template<typename T>
    std::string ToString( const T * const a, const size_t n, const int p = 16)
    {
        return Array1DToString<T>(p)(a, n);
    }
    
    
    template<typename T>
    std::string ToString( const std::vector<T> & a, const int p = 16)
    {
        return ToString( a.data(), a.size(), p );
    }
    
} // namespace Tools
