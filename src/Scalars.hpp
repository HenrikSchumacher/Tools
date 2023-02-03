#include <complex>

namespace Tools
{
    enum class ScalarFlag : char
    {
        Generic = 2,
        Plus    = 1,
        Minus   = -1,
        Zero    = 0
    };
    

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
        using Real    = T;
        
        using Complex = std::complex<T>;
        
        int Precision = 0;
        
        static constexpr bool IsReal    = false;
        
        static constexpr bool IsComplex = false;
        
        static constexpr bool IsScalar  = false;
    };

    template <typename T>
    struct ScalarTraits<std::complex<T>>
    {
        using Real    = T;
        
        using Complex = std::complex<T>;
        
        int Precision = 0;
        
        static constexpr bool IsReal    = false;
        
        static constexpr bool IsComplex = false;
        
        static constexpr bool IsScalar  = false;
    };
    
    
    template<> struct ScalarTraits<Real32>
    {
        using Real    = Real32;
        
        using Complex = Complex32;
        
        int Precision = 32;
        
        static constexpr bool IsReal    = true;
        
        static constexpr bool IsComplex = false;
        
        static constexpr bool IsScalar  = true;
    };
    
    template<> struct ScalarTraits<Complex32>
    {
        using Real    = Real32;
        
        using Complex = Complex32;
        
        int Precision = 32;
        
        static constexpr bool IsReal    = false;
        
        static constexpr bool IsComplex = true;
        
        static constexpr bool IsScalar  = true;
    };
    
    template<> struct ScalarTraits<Real64>
    {
        using Real    = Real64;
        
        using Complex = Complex64;
        
        int Precision = 64;
        
        static constexpr bool IsReal    = true;
        
        static constexpr bool IsComplex = false;
        
        static constexpr bool IsScalar  = true;
    };
    
    template<> struct ScalarTraits<Complex64>
    {
        using Real    = Real64;
        
        using Complex = Complex64;
        
        int Precision = 64;
        
        static constexpr bool IsReal    = false;
        
        static constexpr bool IsComplex = true;
        
        static constexpr bool IsScalar  = true;
    };
    
    template<> struct ScalarTraits<Real128>
    {
        using Real    = Real128;
        
        using Complex = Complex128;
        
        int Precision = 128;
        
        static constexpr bool IsReal    = true;
        
        static constexpr bool IsComplex = false;
        
        static constexpr bool IsScalar  = true;
    };
    
    template<> struct ScalarTraits<Complex128>
    {
        using Real    = Real128;
        
        using Complex = Complex128;
        
        int Precision = 128;
        
        static constexpr bool IsReal    = false;
        
        static constexpr bool IsComplex = true;
        
        static constexpr bool IsScalar  = true;
    };
    
    // scalar_cast<T>(x) cast x to the precision of T, but preserves Real/Complex
    template<typename T, typename S>
    constexpr force_inline
    std::enable_if_t<
        ScalarTraits<T>::IsScalar
        &&
        ScalarTraits<S>::IsReal,
        typename ScalarTraits<T>::Real
    >
    scalar_cast( S x )
    {
        return static_cast<typename ScalarTraits<T>::Real>(x);
    }

    template<typename T, typename S>
    constexpr force_inline
    std::enable_if_t<
        ScalarTraits<T>::IsScalar
        &&
        ScalarTraits<S>::IsComplex,
        typename ScalarTraits<T>::Complex
    >
    scalar_cast( S x )
    {
        return static_cast<typename ScalarTraits<T>::Complex>(x);
    }
    
    
    
    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T, but preserves Real/Complex
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        ScalarTraits<S>::IsScalar && ScalarTraits<T>::IsScalar
        &&
        ScalarTraits<R>::IsReal
        &&
        std::less_equal(ScalarTraits<S>::Precision,ScalarTraits<T>::Precision),
        typename ScalarTraits<S>::Real
    >
    lo_prec_cast( R x )
    {
        return static_cast<typename ScalarTraits<S>::Real>(x);
    }
    
    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        ScalarTraits<S>::IsScalar && ScalarTraits<T>::IsScalar
        &&
        ScalarTraits<R>::IsReal
        &&
        std::greater(ScalarTraits<S>::Precision, ScalarTraits<T>::Precision),
        typename ScalarTraits<T>::Real
    >
    lo_prec_cast( R x )
    {
        return static_cast<typename ScalarTraits<T>::Real>(x);
    }
    
    
    
    // hi_prec_cast<S,T>(x) casts x to the higher precision of S and T, but preserves Real/Complex
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        ScalarTraits<S>::IsScalar && ScalarTraits<T>::IsScalar
        &&
        ScalarTraits<R>::IsReal
        &&
        std::greater_equal(ScalarTraits<S>::Precision,ScalarTraits<T>::Precision),
        typename ScalarTraits<S>::Real
    >
    hi_prec_cast( R x )
    {
        return static_cast<typename ScalarTraits<S>::Real>(x);
    }
    
    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        ScalarTraits<S>::IsScalar && ScalarTraits<T>::IsScalar
        &&
        ScalarTraits<R>::IsReal
        &&
        std::less(ScalarTraits<S>::Precision, ScalarTraits<T>::Precision),
        typename ScalarTraits<T>::Real
    >
    hi_prec_cast( R x )
    {
        return static_cast<typename ScalarTraits<T>::Real>(x);
    }
    
    
    
    
    template<
        ScalarFlag alpha_flag, ScalarFlag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    force_inline void combine_scalars( const R_0 alpha, const S_0 & x, const R_1 beta, S_1 & y  )
    {
        if constexpr ( alpha_flag == ScalarFlag::Plus )
        {
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                y = scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                y += scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                y = scalar_cast<S_1>(x) - y;
            }
            else
            {
                y = scalar_cast<S_1>(beta) * scalar_cast<S_1>(x) - y;
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Minus )
        {
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                y = -scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                y -= scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                y = -y - scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                y = scalar_cast<S_1>(beta) * y - scalar_cast<S_1>(x);
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Zero )
        {
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                y = static_cast<S_1>(0);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                // do nothing;
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                y = -y;
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                y *= scalar_cast<S_1>(beta);
            }
        }
        else if constexpr ( alpha_flag == ScalarFlag::Generic )
        {
            // TODO: This assumes that R_1 and S_1 are compatible.
            // TODO: This will do the computation in better precisiono of alpha and x!!!
            
            if constexpr ( beta_flag == ScalarFlag::Zero )
            {
                y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Plus )
            {
                // TODO: This assumes that R_1 and S_1 are compatible.
                // TODO: This will do the computation in better precisiono of alpha and x!!!
                y += scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x);
            }
            else if constexpr ( beta_flag == ScalarFlag::Minus )
            {
                y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) - y;
            }
            else if constexpr ( beta_flag == ScalarFlag::Generic )
            {
                y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) + scalar_cast<S_1>(beta) * y;
            }
        }
    }
}
