#pragma once

namespace Tools
{
    template <typename T>
    [[nodiscard]] std::string ToString( const T & a_value, const int prec = 17 )
    {
        std::stringstream s;
        s << std::setprecision(prec);
        s << std::showpoint;
        s << ToUnderlying(a_value);
        return s.str();
    }

    template <typename T>
    std::ostream & operator<<( std::ostream & sout, const std::complex<T> & z )
    {
        sout << std::real(z) << ((std::imag(z) >= static_cast<T>(0)) ? " + " : " - ") << std::abs(std::imag(z)) << " I" ;
        
        return sout;
    }
    
//    template <typename T>
//    std::ostream & operator<<( std::ostream & sout, const T & val )
//    {
//        return (sout << ToUnderlying(val));
//    }
    
    
    template <typename T>
    [[nodiscard]] std::string ToString( const std::complex<T> & z, const int prec = 17 )
    {
        std::stringstream s;
        s << std::setprecision(prec);
        s << std::showpoint;
        s << z;
        return s.str();
    }
    
    template <typename T>
    [[nodiscard]] std::string ToString( const signed char & z )
    {
        std::stringstream sout;
        sout << static_cast<int>(z);
        return sout.str();
    }
    
    template <typename T>
    [[nodiscard]] std::string ToString( const unsigned char & z )
    {
        std::stringstream sout;
        sout << static_cast<unsigned int>(z);
        return sout.str();
    }
    
    template <typename S, typename T>
    [[nodiscard]] std::string ToString( const std::pair<S,T> & p )
    {
        std::stringstream sout;
        sout << "{ " << p.first << ", " << p.second << " } ";
        return sout.str();
    }
    
    template<typename Scal, typename Int, typename Int2, typename Int3, class Stream_T>
    Stream_T & ArrayToStream(
        const Scal * const a,
        const Int  * const dims,
        const Int2 * const lds,
        Int3 rank,
        Stream_T & s,
        std::string line_prefix
    )
    {
        if( rank <= 0 )
        {
            s << a[0];
        }
        else if( rank == 1 )
        {
            s << line_prefix << "{ ";
            
            if( dims[0] > 0 )
            {
                s << a[0];
            }
            
            for( Int i = 1; i < dims[0]; ++i )
            {
                s << ", " << a[i];
            }
            
            s << " }";
        }
        else
        {
            std::string new_line_prefix = line_prefix+"\t";
            
            s << line_prefix << "{" << "\n";
            if( dims[0] > 0 )
            {
                ArrayToStream( a, &dims[1], &lds[1], rank-1, s, new_line_prefix );
            }
            
            for( Int i = 1; i < dims[0]; ++i )
            {
                s << ","  << "\n";
                ArrayToStream( &a[lds[0]*i], &dims[1], &lds[1], rank-1, s, new_line_prefix );
            }
            
            s << "\n";
            
            s << line_prefix << "}";
        }
        
        return s;
    }
    
    
    template<typename Scal, typename Int, typename Int2, class Stream_T>
    Stream_T & ArrayToStream(
        const Scal * const a,
        const Int  * const dims,
        Int2 rank,
        Stream_T & s,
        std::string line_prefix = std::string("")
    )
    {
        if( rank >= 0 )
        {
            std::vector<Int> lds (rank);

            if( rank >= 1 )
            {
                lds[rank-1] = 1;
                
                for( Size_T i = rank-1; i --> 0;  )
                {
                    lds[i] = lds[i+1] * dims[i+1];
                }
            }
            
            ArrayToStream( a, dims, lds.data(), rank, s, line_prefix );
        }
        return s;
    }
    
    template<typename T, typename Int, typename Int2>
    std::string ArrayToString(
        const T   * const a,
        const Int * const dims,
        Int2 rank,
        int prec = 17,
        std::string line_prefix = ""
    )
    {
        std::stringstream s;
        
        s << std::setprecision(prec);
        s << std::showpoint;
        
        (void)ArrayToStream<T,Int>( a, dims, rank, s, line_prefix ).str();

        return s.str();
    }
    
    template<typename Scal, typename Int>
    std::string ArrayToString(
        const Scal * const a,
        std::initializer_list<Int> dims,
        int prec = 17,
        std::string line_prefix = ""
    )
    {
        return ArrayToString( a, &*dims.begin(), dims.size(), prec, line_prefix );
    }
    
    template<typename T>
    [[nodiscard]] std::string ToString( const std::vector<T> & v )
    {
        const Size_T dim = v.size();
        return ArrayToString( &v[0], &dim, 1 );
    }
    
    template<Size_T n, typename T>
    [[nodiscard]] std::string ToString( const std::array<T,n> & v )
    {
        const Size_T dim = n;
        return ArrayToString( &v[0], &dim, 1 );
    }
    
    
    template<typename T>
    [[nodiscard]] std::string MatrixString(
        const Size_T m, const Size_T n, cref<T> A, const Size_T ldA,
        cref<std::string> header,
        cref<std::string> row_prefix,
        cref<std::string> row_infix,
        cref<std::string> row_suffix,
        cref<std::string> linesep,
        cref<std::string> footer,
        const int prec = 17
    )
    {
        std::stringstream s;
        
        s << std::setprecision(prec);
        s << std::showpoint;

        s << header;

        for( Size_T i = 0; i < (m-1); i++)
        {
            s << row_prefix;

            for( Size_T j = 0; j < n-1; j++)
            {
                s << A[ldA * i + j] << row_infix;
            }

            s     << A[ldA * i + (n-1)] << row_suffix;
            
            s << linesep;
        }
        {
            s << row_prefix;

            for( Size_T j = 0; j < n-1; j++)
            {
                s << A[ldA * (m-1) + j] << row_infix;
            }
            s     << A[ldA * (m-1) + (n-1)] << row_suffix;
        }

        s << footer;

        return s.str();
    }
    
    template<typename T>
    [[nodiscard]] std::string MatrixStringTSV(
        const Size_T m, const Size_T n, cref<T> A, const Size_T ldA,
        int prec = 17
    )
    {
        return MatrixString( m, n, A, ldA, "", "", "\t", "", "\n", "", prec );
    }
    
    template<typename T>
    [[nodiscard]] std::string MatrixStringCSV(
        const Size_T m, const Size_T n, cref<T> A, const Size_T ldA,
        int prec = 17
    )
    {
        return MatrixString( m, n, A, ldA, "", "", ",", "", "\n", "", prec );
    }
    
    
    template<typename T>
    [[nodiscard]] std::string StringWithLeadingZeroes(
        const T val, const int width
    )
    {
        std::stringstream s;
        s << std::setw(width) << std::setfill('0') << val;
        return s.str();
    }

    
} // namespace Tools
