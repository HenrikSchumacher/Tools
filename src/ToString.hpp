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
    std::ostream & operator<<( std::ostream & sout, const std::complex<T> & z )
    {
        sout << real(z) << ((imag(z) >= static_cast<T>(0)) ? " + " : " - ") << std::abs(imag(z)) << " I" ;
        
        return sout;
    }
    
    template <typename T>
    std::string ToString(const std::complex<T> & z, const int p = 16)
    {
        std::stringstream sout;
        sout.precision(p);
        sout << z;
        return sout.str();
    }
    
    template<typename Scalar, typename Int, class Stream_T>
    Stream_T & ArrayToStream(
        const Scalar * const a,
        const Int * const dims,
        const Int * const lds,
        Int rank,
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
            
            s << line_prefix << "{" << std::endl;
            if( dims[0] > 0 )
            {
                ArrayToStream( a, &dims[1], &lds[1], rank-1, s, new_line_prefix );
            }
            
            for( Int i = 1; i < dims[0]; ++i )
            {
                s << ","  << std::endl;
                ArrayToStream( &a[lds[0]*i], &dims[1], &lds[1], rank-1, s, new_line_prefix );
            }
            
            s << std::endl;
            
            s << line_prefix << "}";
        }
        
        return s;
    }
    
    
    template<typename Scalar, typename Int, class Stream_T>
    Stream_T & ArrayToStream(
        const Scalar * const a,
        const Int    * const dims,
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
                
                for( std::size_t i = rank-1; i --> 0;  )
                {
                    lds[i] = lds[i+1] * dims[i+1];
                }
            }
            ArrayToStream( a, dims, lds.data(), rank, s, line_prefix );
        }
        return s;
    }
    
    template<typename Scalar, typename Int, typename Int2, typename Int3>
    std::string ArrayToString(
        const Scalar * const a,
        const Int    * const dims,
        Int2 rank,
        Int3 prec = 16
    )
    {
        std::stringstream s;
        
        s << std::setprecision(prec);
        
        (void)ArrayToStream<Scalar,Int>( a, dims, rank, s, std::string("") ).str();

        return s.str();
    }
    
    template<typename T>
    std::string ToString( const std::vector<T> & v )
    {
        const size_t dims [0] = v.size();
        return ArrayToString(v.data(), dims, 1);
    }
} // namespace Tools
