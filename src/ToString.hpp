#pragma once

namespace Tools
{
    [[nodiscard]] std::string ToString( const std::string & s )
    {
        return s;
    }
    
    [[nodiscard]] std::string ToString( const char * ptr )
    {
        return std::string(ptr);
    }
    
    [[nodiscard]] std::string ToString( char * ptr )
    {
        return std::string(ptr);
    }
    
    [[nodiscard]] std::string ToString( const double & value )
    {
        return std::format("{: .16e}",value);
    }
    
    [[nodiscard]] std::string ToString( const float & value )
    {
        return std::format("{: .7e}",value);
    }
    
    [[nodiscard]] std::string ToString( const std::complex<double> & z )
    {
        return std::format("{: .16e} {:+.16e}I",std::real(z),std::imag(z));
    }
                           
    [[nodiscard]] std::string ToString( const std::complex<float> & z )
    {
        return std::format("{: .7e} {:+.7e}I",std::real(z),std::imag(z));
    }
    
    
    [[nodiscard]] std::string ToStringFPGeneral( const double & value )
    {
        return std::format("{:.16g}",value);
    }
    
    [[nodiscard]] std::string ToStringFPGeneral( const float & value )
    {
        return std::format("{:.7g}",value);
    }
    
    [[nodiscard]] std::string ToStringFPGeneral( const std::complex<double> & z )
    {
        return std::format("{:.16g} {:+.16g}I",std::real(z),std::imag(z));
    }
    
    [[nodiscard]] std::string ToStringFPGeneral( const std::complex<float> & z )
    {
        return std::format("{:.7g} {:+.7g}I",std::real(z),std::imag(z));
    }
    
    
    [[nodiscard]] std::string ToMathematicaScientificString( const float x )
    {
        std::string s = ToString(x);
        
        Size_T p = s.find("e");
        
        if(p != std::string::npos)
        {
            s.replace(p, 1, "*^");
        }
//        std::cout << s << std::endl;
        return s;
    }
    
    [[nodiscard]] std::string ToMathematicaScientificString( const double x )
    {
        std::string s = ToString(x);
        
        Size_T p = s.find("e");
        
        if(p != std::string::npos)
        {
            s.replace(p, 1, "*^");
        }
//        std::cout << s << std::endl;
        return s;
    }
    
    [[nodiscard]] std::string ToMathematicaString( const float x )
    {
        std::string s = ToStringFPGeneral(x);
        
        Size_T p = s.find("e");
        
        if(p != std::string::npos)
        {
            s.replace(p, 1, "*^");
        }
//        std::cout << s << std::endl;
        return s;
    }
    
    [[nodiscard]] std::string ToMathematicaString( const double x )
    {
        std::string s = ToStringFPGeneral(x);
        
        Size_T p = s.find("e");
        
        if(p != std::string::npos)
        {
            s.replace(p, 1, "*^");
        }
//        std::cout << s << std::endl;
        return s;
    }
    
    template <typename T>
    std::ostream & operator<<( std::ostream & sout, const std::complex<T> & z )
    {
        return sout << ToString(z);
    }

    template <typename T>
    [[nodiscard]] std::string ToString( T * ptr )
    {
        return std::format( "{:p}", (void *)ptr );
    }
    
    template <typename T>
    [[nodiscard]] std::string ToString( T const * ptr )
    {
    std::cout << "ToString(T const * const ptr)" << std::endl;
        
        return std::format( "{:p}", (void const *)ptr );
    }

    template <typename T>
    [[nodiscard]] std::enable_if_t<IntQ<T>,std::string> ToString( const T & value )
    {
        return std::format("{:d}",value);
    }


    
    template<typename T>
    static constexpr bool ClassEnumQ = std::is_enum<T>::value && !std::is_convertible<T,int>::value;
    
    
    template <typename T>
    [[nodiscard]] std::enable_if_t<ClassEnumQ<T>,std::string> ToString( const T & value )
    {
        return ToString( ToUnderlying(value) );
    }
    
    template <typename S, typename T>
    [[nodiscard]] std::string ToString( const std::pair<S,T> & p )
    {
        return std::string("{ ") + ToString(p.first) + ", " + ToString(p.second) + " }";
    }
    
    template<typename Scal, typename Int, typename Int2, typename Int3>
    [[nodiscard]] std::string ArrayToString(
        const Scal * const a,
        const Int  * const dims,
        const Int2 * const lds,
        Int3 rank,
        std::string line_prefix
    )
    {
        std::string s;
        
        if( rank <= 0 )
        {
            s += ToString(a[0]);
        }
        else if( rank == 1 )
        {
            s += line_prefix;
            s += "{ ";
            
            if( dims[0] > 0 )
            {
                s += ToString(a[0]);
            }
            
            for( Int i = 1; i < dims[0]; ++i )
            {
                s += ", ";
                s += ToString(a[i]);
            }
            
            s += " }";
        }
        else
        {
            std::string new_line_prefix = line_prefix + "\t";
            
            s += line_prefix;
            s += "{\n";
            
            if( dims[0] > 0 )
            {
                s += ArrayToString( a, &dims[1], &lds[1], rank-1, new_line_prefix );
            }
            
            for( Int i = 1; i < dims[0]; ++i )
            {
                s += ",\n";
                s += ArrayToString( &a[lds[0]*i], &dims[1], &lds[1], rank-1, new_line_prefix );
            }
            
            s += "\n";
            
            s += line_prefix;
            s += "}";
        }
        
        return s;
    }
    
    
    template<typename Scal, typename Int, typename Int2>
    [[nodiscard]] std::string ArrayToString(
        const Scal * const a,
        const Int  * const dims,
        Int2 rank,
        std::string line_prefix = std::string("")
    )
    {
        if( rank >= 0 )
        {
            const Size_T r = ToSize_T(rank);
            
            std::vector<Int> lds (r);

            if( r >= 1 )
            {
                lds[r-1] = 1;
                
                for( Size_T i = r-1; i --> 0;  )
                {
                    lds[i] = lds[i+1] * dims[i+1];
                }
            }
            
            return ArrayToString( a, dims, lds.data(), rank, line_prefix );
        }
        else
        {
            return std::string();
        }
    }
    
    template<typename Scal, typename Int>
    [[nodiscard]] std::string ArrayToString(
        const Scal * const a,
        std::initializer_list<Int> dims,
        std::string line_prefix = ""
    )
    {
        return ArrayToString( a, &*dims.begin(), dims.size(), line_prefix );
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
    

    template<Size_T N_ = VarSize, typename T, typename Int = Size_T>
    [[nodiscard]] std::string VectorString(
        cref<T> A,
        cref<std::string> prefix,
        cref<std::string> infix,
        cref<std::string> suffix,
        const Int n = static_cast<Int>(N_)
    )
    {
        constexpr Int N = static_cast<Int>(N_);
        
        std::string s;
        
        s += prefix;
        
        if( ((N_ > VarSize) ? N : n) > Int(0) )
        {
            s += ToString(A[0]);
        }
        
        for( Int i = 1; i < ((N_ > VarSize) ? N : n); ++i )
        {
            s += infix;
            s += ToString(A[i]);
        }
        
        s  += suffix;

        return s;
    }
    
    template<Size_T M_ = VarSize, Size_T N_ = VarSize, typename T, typename Int = Size_T>
    [[nodiscard]] std::string MatrixString(
        cref<T> A, const Size_T ldA,
        cref<std::string> header,
        cref<std::string> row_prefix,
        cref<std::string> row_infix,
        cref<std::string> row_suffix,
        cref<std::string> row_sep,
        cref<std::string> footer,
        const Int m = static_cast<Int>(M_),
        const Int n = static_cast<Int>(N_)
    )
    {
        constexpr Int M = static_cast<Int>(M_);
        constexpr Int N = static_cast<Int>(N_);
        
        std::string s;
        
        s += header;
        
        if( ((M_ > VarSize) ? M : m) > Int(0) )
        {
            s += VectorString<N>(&A[0],row_prefix,row_infix,row_suffix,n);
        }
        
        for( Int i = 1; i < ((M_ > VarSize) ? M : m); ++i )
        {
            s += row_sep;
            
            s += VectorString<N>(&A[ldA * i],row_prefix,row_infix,row_suffix,n);
        }

        s += footer;
        
        return s;
    }
    
    template<typename T>
    [[nodiscard]] std::string MatrixStringTSV(
        const Size_T m, const Size_T n, cref<T> A, const Size_T ldA
    )
    {
        return MatrixString( A, ldA, "", "", "\t", "", "\n", "", m, n );
    }
    
    template<typename T>
    [[nodiscard]] std::string MatrixStringCSV(
        const Size_T m, const Size_T n, cref<T> A, const Size_T ldA
    )
    {
        return MatrixString( A, ldA, "", "", ",", "", "\n", "", m, n );
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
