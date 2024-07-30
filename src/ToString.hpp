#pragma once

namespace Tools
{
    template <typename T>
    [[nodiscard]] std::string ToString( const T & a_value, const int p = 16 )
    {
        std::stringstream sout;
        sout.precision(p);
        sout << ToUnderlying(a_value);
        return sout.str();
    }

    template <typename T>
    std::ostream & operator<<( std::ostream & sout, const std::complex<T> & z )
    {
        sout << std::real(z) << ((std::imag(z) >= static_cast<T>(0)) ? " + " : " - ") << std::abs(std::imag(z)) << " I" ;
        
        return sout;
    }
    
    template <typename T>
    [[nodiscard]] std::string ToString( const std::complex<T> & z, const int p = 16)
    {
        std::stringstream sout;
        sout.precision(p);
        sout << z;
        return sout.str();
    }
    
    template <typename T>
    [[nodiscard]] std::string ToString( const signed char & z )
    {
        std::stringstream sout;
        sout << static_cast<int>(z);
        return sout.str();
    }
    
    template<typename Scal, typename Int, class Stream_T>
    Stream_T & ArrayToStream(
        const Scal * const a,
        const Int * const dims,
        const Int * const lds,
        Int rank,
        Stream_T & s,
        std::string line_prefix
    )
    {
        if( rank <= 0 )
        {
            s << ToUnderlying(a[0]);
        }
        else if( rank == 1 )
        {
            s << line_prefix << "{ ";
            
            if( dims[0] > 0 )
            {
                s << ToUnderlying(a[0]);
            }
            
            for( Int i = 1; i < dims[0]; ++i )
            {
                s << ", " << ToUnderlying(a[i]);
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
    
    
    template<typename Scal, typename Int, class Stream_T>
    Stream_T & ArrayToStream(
        const Scal * const a,
        const Int  * const dims,
        Int rank,
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
    
    template<typename Scal, typename Int, typename Int2>
    std::string ArrayToString(
        const Scal * const a,
        const Int  * const dims,
        Int rank,
        int prec = 16
    )
    {
        std::stringstream s;
        
        s << std::setprecision(prec);
        
        (void)ArrayToStream<Scal,Int>( a, dims, rank, s, std::string("") ).str();

        return s.str();
    }
    
    template<typename Scal, typename Int>
    std::string ArrayToString(
        const Scal * const a,
        std::initializer_list<Int> dims,
        int prec = 16
    )
    {
        return ArrayToString( a, &*dims.begin(), dims.size(), prec );
    }
    
    template<typename T>
    [[nodiscard]] std::string ToString( const std::vector<T> & v )
    {
        const Size_T dim = v.size();
        return ArrayToString( &v[0], &dim, 1);
    }
} // namespace Tools
