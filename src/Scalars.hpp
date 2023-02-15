#include <complex>

namespace Tools
{
    namespace Scalar
    {
        
        enum class Flag : char
        {
            Generic =  2,
            Plus    =  1,
            Minus   = -1,
            Zero    =  0
        };
        
    } // namespace Scalar
    
    
    std::string ToString( Scalar::Flag flag )
    {
        switch( flag )
        {
            case Scalar::Flag::Plus:
            {
                return "+";
            }
            case Scalar::Flag::Zero:
            {
                return "0";
            }
            case Scalar::Flag::Minus:
            {
                return "-";
            }
            default:
            {
                return "G";
            }
        }
    }

    
    using Int16  = int16_t;
    using Int32  = int32_t;
    using Int64  = int64_t;
    
    using UInt16  = uint16_t;
    using UInt32  = uint32_t;
    using UInt64  = uint64_t;
    
    using Real32  = float;
    using Real64  = double;
    using Real128 = long double;
    
    using Complex32  = std::complex<Real32>;
    using Complex64  = std::complex<Real64>;
    using Complex128 = std::complex<Real128>;
    
    
    template<typename T>
    force_inline T constexpr conj( const T & x )
    {
        return x;
    }
    
    template<typename T>
    force_inline T constexpr real( const T & x )
    {
        return x;
    }
    
    template<typename T>
    force_inline constexpr T imag( const T & x )
    {
        return static_cast<T>(0);
    }
    
    template<typename T>
    force_inline T constexpr abs_squared( const T & x )
    {
        return  x * x;
    }
    
    template<typename T>
    force_inline T constexpr abs_squared( const std::complex<T> & z )
    {
        const T x = real(z);
        const T y = imag(z);
        return  x * x + y * y;
    }

    namespace Scalar
    {
        template<typename T> inline constexpr const bool IsComplex = false;
        
        template<> inline constexpr const bool IsComplex<Real32 >    = false;
        template<> inline constexpr const bool IsComplex<Real64 >    = false;
        template<> inline constexpr const bool IsComplex<Real128>    = false;
        template<> inline constexpr const bool IsComplex<Complex32 > = true;
        template<> inline constexpr const bool IsComplex<Complex64 > = true;
        template<> inline constexpr const bool IsComplex<Complex128> = true;
        
        template<typename T> inline constexpr const bool IsReal = false;
        
        template<> inline constexpr const bool IsReal<Real32 >       = true;
        template<> inline constexpr const bool IsReal<Real64 >       = true;
        template<> inline constexpr const bool IsReal<Real128>       = true;
        template<> inline constexpr const bool IsReal<Complex32 >    = false;
        template<> inline constexpr const bool IsReal<Complex64 >    = false;
        template<> inline constexpr const bool IsReal<Complex128>    = false;
        
        template<typename T> inline constexpr const bool IsScalar = false;
        
        template<> inline constexpr const bool IsScalar<Real32 >     = true;
        template<> inline constexpr const bool IsScalar<Real64 >     = true;
        template<> inline constexpr const bool IsScalar<Real128>     = true;
        template<> inline constexpr const bool IsScalar<Complex32 >  = true;
        template<> inline constexpr const bool IsScalar<Complex64 >  = true;
        template<> inline constexpr const bool IsScalar<Complex128>  = true;
        
        template<typename T> inline constexpr const int Prec     = 0;
        
        template<> inline constexpr const uint Prec<Real32 >     = 32u;
        template<> inline constexpr const uint Prec<Real64 >     = 64u;
        template<> inline constexpr const uint Prec<Real128>     = 128u;
        template<> inline constexpr const uint Prec<Complex32 >  = 32u;
        template<> inline constexpr const uint Prec<Complex64 >  = 64u;
        template<> inline constexpr const uint Prec<Complex128>  = 128u;
        
        template<> inline constexpr const uint Prec<Int16 >      = 16u;
        template<> inline constexpr const uint Prec<Int32 >      = 32u;
        template<> inline constexpr const uint Prec<Int64 >      = 64u;
        template<> inline constexpr const uint Prec<UInt16>      = 16u;
        template<> inline constexpr const uint Prec<UInt32>      = 32u;
        template<> inline constexpr const uint Prec<UInt64>      = 64u;
        
        
        template<typename T>
        using Real =
            std::conditional_t<Prec<T> ==  32u,  Real32,
            std::conditional_t<Prec<T> ==  64u,  Real64,
            std::conditional_t<Prec<T> == 128u, Real128,
            T>>>;
        
        template<typename T>
        using Complex =
            std::conditional_t<Prec<T> ==  32u,  Complex32,
            std::conditional_t<Prec<T> ==  64u,  Complex64,
            std::conditional_t<Prec<T> == 128u, Complex128,
            T>>>;
        
        
        template<typename T> inline constexpr const Scalar::Real<T>    Zero {0};
        template<typename T> inline constexpr const Scalar::Real<T>    One  {1};
        template<typename T> inline constexpr const Scalar::Real<T>    Two  {2};
        template<typename T> inline constexpr const Scalar::Real<T>    Half = One<T>/Two<T>;
        
        template<typename T> inline constexpr const Scalar::Complex<T> I {0,1};
        
        template<typename T> inline constexpr const Scalar::Real<T> Pi = 3.141592653589793238462643383279502884197;
        
        template<typename T> inline constexpr const Scalar::Real<T> eps   = std::numeric_limits<Scalar::Real<T>>::epsilon();
        template<typename T> inline constexpr const Scalar::Real<T> Max   = std::numeric_limits<Scalar::Real<T>>::max();
        template<typename T> inline constexpr const Scalar::Real<T> Infty = std::numeric_limits<Scalar::Real<T>>::max();
        template<typename T> inline constexpr const Scalar::Real<T> Min   = std::numeric_limits<Scalar::Real<T>>::lowest();
//
  
//        template<typename R, typename S, typename T>
//        force_inline constexpr R Fraction( const S & a, const T & b )
//        {
//            return scalar_cast<R>(a)/scalar_cast<R>(b);
//        }
        
//        template<> inline constexpr const bool IsComplex<Complex32 > = true;
//        template<> inline constexpr const bool IsComplex<Complex64 > = true;
//        template<> inline constexpr const bool IsComplex<Complex128> = true;
     

        
    } // namespace Scalar
    
    // scalar_cast<T>(x) cast x to the precision of T, but preserves Real/Complex
    template<typename T, typename S>
    constexpr force_inline
    std::enable_if_t<
        Scalar::IsScalar<T>
        &&
        Scalar::IsReal<S>,
        typename Scalar::Real<T>
    >
    scalar_cast( S x )
    {
        return static_cast<typename Scalar::Real<T>>(x);
    }

    template<typename T, typename S>
    constexpr force_inline
    std::enable_if_t<
        Scalar::IsScalar<T>
        &&
        Scalar::IsComplex<S>,
        typename Scalar::Complex<T>
    >
    scalar_cast( S x )
    {
        return static_cast<typename Scalar::Complex<T>>(x);
    }
    
    
    
    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T, but preserves Real/Complex
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        Scalar::IsScalar<S> && Scalar::IsScalar<T>
        &&
        Scalar::IsReal<R>
        &&
        std::less_equal(Scalar::Prec<S>,Scalar::Prec<T>),
        typename Scalar::Real<S>
    >
    lo_prec_cast( R x )
    {
        return static_cast<typename Scalar::Real<S>>(x);
    }
    
    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        Scalar::IsScalar<S> && Scalar::IsScalar<T>
        &&
        Scalar::IsReal<R>
        &&
        std::greater(Scalar::Prec<S>, Scalar::Prec<T>),
        typename Scalar::Real<T>
    >
    lo_prec_cast( R x )
    {
        return static_cast<typename Scalar::Real<T>>(x);
    }
    
    
    
    // hi_prec_cast<S,T>(x) casts x to the higher precision of S and T, but preserves Real/Complex
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        Scalar::IsScalar<S> && Scalar::IsScalar<T>
        &&
        Scalar::IsReal<R>
        &&
        std::greater_equal(Scalar::Prec<S>,Scalar::Prec<T>),
        typename Scalar::Real<S>
    >
    hi_prec_cast( R x )
    {
        return static_cast<typename Scalar::Real<S>>(x);
    }
    
    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T
    template<typename S, typename T, typename R>
    constexpr force_inline
    std::enable_if_t<
        Scalar::IsScalar<S> && Scalar::IsScalar<T>
        &&
        Scalar::IsReal<R>
        &&
        std::less(Scalar::Prec<S>, Scalar::Prec<T>),
        typename Scalar::Real<T>
    >
    hi_prec_cast( R x )
    {
        return static_cast<typename Scalar::Real<T>>(x);
        
    }
    
#define ASSERT_REAL(R) static_assert( Scalar::IsReal<R>, "Template parameter " #R " must be an integral type." );
    
#define ASSERT_COMPLEX(C) static_assert( Scalar::IsComplex<C>, "Template parameter " #C " must be an integral type." );
}
