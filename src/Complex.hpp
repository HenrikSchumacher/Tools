namespace Tools
{
    template<typename T>
    force_inline T conj( const T x )
    {
        return x;
    }
    
    template<typename T>
    force_inline T real( const T x )
    {
        return x;
    }
    
    template<typename T>
    force_inline constexpr T imag( const T x )
    {
        return static_cast<T>(0);
    }
    
    template<typename T>
    force_inline T abs_squared( const T x )
    {
        return  x * x;
    }
    
//        template<typename T>
//        force_inline T abs_squared( const std::complex<T> z )
//        {
//            return  norm(z);
//        }
    
    template<typename T>
    force_inline T abs_squared( const std::complex<T> z )
    {
        const T x = real(z);
        const T y = imag(z);
        return  x * x + y * y;
    }
    
    template <typename T>
    struct ScalarTraits
    {
        using RealType = T;
        
        static constexpr bool IsComplex = false;
    };

    template <typename T>
    struct ScalarTraits<std::complex<T>>
    {
        using RealType = T;
        
        static constexpr bool IsComplex = true;
    };
}

 
