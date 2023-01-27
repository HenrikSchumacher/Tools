#include <complex>

namespace Tools
{
    using Real32  = float;
    using Real64  = double;
    using Real128 = long double;
    
    using Int32  = int32_t;
    using Int64  = int64_t;
    
    using Complex32  = std::complex<Real32>;
    using Complex64  = std::complex<Real64>;
    using Complex128 = std::complex<Real128>;
    
    
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
        using Real = T;
        
        static constexpr bool IsComplex = false;
    };

    template <typename T>
    struct ScalarTraits<std::complex<T>>
    {
        using Real = T;
        
        static constexpr bool IsComplex = true;
    };
    
    template <typename S, typename T>
    struct ScalarCast {};
    
    template<> struct ScalarCast<Real32, Real32 > { using Type = Real32;  };
    template<> struct ScalarCast<Real64, Real32 > { using Type = Real32;  };
    template<> struct ScalarCast<Real128,Real32 > { using Type = Real32;  };
    
    template<> struct ScalarCast<Real32, Real64 > { using Type = Real64;  };
    template<> struct ScalarCast<Real64, Real64 > { using Type = Real64;  };
    template<> struct ScalarCast<Real128,Real64 > { using Type = Real64;  };
    
    template<> struct ScalarCast<Real32, Real128> { using Type = Real128;  };
    template<> struct ScalarCast<Real64, Real128> { using Type = Real128;  };
    template<> struct ScalarCast<Real128,Real128> { using Type = Real128;  };
    
    
    template<> struct ScalarCast<Real32, Complex32 > { using Type = Real32;  };
    template<> struct ScalarCast<Real64, Complex32 > { using Type = Real32;  };
    template<> struct ScalarCast<Real128,Complex32 > { using Type = Real32;  };
    
    template<> struct ScalarCast<Real32, Complex64 > { using Type = Real64;  };
    template<> struct ScalarCast<Real64, Complex64 > { using Type = Real64;  };
    template<> struct ScalarCast<Real128,Complex64 > { using Type = Real64;  };
    
    template<> struct ScalarCast<Real32, Complex128> { using Type = Real128;  };
    template<> struct ScalarCast<Real64, Complex128> { using Type = Real128;  };
    template<> struct ScalarCast<Real128,Complex128> { using Type = Real128;  };
    
    
    template<> struct ScalarCast<Complex32, Complex32 > { using Type = Complex32;  };
    template<> struct ScalarCast<Complex64, Complex32 > { using Type = Complex32;  };
    template<> struct ScalarCast<Complex128,Complex32 > { using Type = Complex32;  };
    
    template<> struct ScalarCast<Complex32, Complex64 > { using Type = Complex64;  };
    template<> struct ScalarCast<Complex64, Complex64 > { using Type = Complex64;  };
    template<> struct ScalarCast<Complex128,Complex64 > { using Type = Complex64;  };
    
    template<> struct ScalarCast<Complex32, Complex128> { using Type = Complex128;  };
    template<> struct ScalarCast<Complex64, Complex128> { using Type = Complex128;  };
    template<> struct ScalarCast<Complex128,Complex128> { using Type = Complex128;  };

    template<typename T>
    constexpr force_inline typename ScalarCast<Real32,T>::Type scalar_cast( Real32 x )
    {
        return static_cast<typename ScalarCast<Real32,T>::Type>(x);
    }
    
    template<typename T>
    constexpr force_inline typename ScalarCast<Real64,T>::Type scalar_cast( Real64 x )
    {
        return static_cast<typename ScalarCast<Real64,T>::Type>(x);
    }
    
    template<typename T>
    constexpr force_inline typename ScalarCast<Real128,T>::Type scalar_cast( Real128 x )
    {
        return static_cast<typename ScalarCast<Real128,T>::Type>(x);
    }
    
    template<typename T>
    constexpr force_inline typename ScalarCast<Complex32,T>::Type scalar_cast( Complex32 x )
    {
        return static_cast<typename ScalarCast<Complex32,T>::Type>(x);
    }
    
    template<typename T>
    constexpr force_inline typename ScalarCast<Complex64,T>::Type scalar_cast( Complex64 x )
    {
        return static_cast<typename ScalarCast<Complex64,T>::Type>(x);
    }
    
    template<typename T>
    constexpr force_inline typename ScalarCast<Complex128,T>::Type scalar_cast( Complex128 x )
    {
        return static_cast<typename ScalarCast<Complex128,T>::Type>(x);
    }
    
}
