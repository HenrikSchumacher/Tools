#include <complex>

namespace Tools
{
    
    using Real32  = float;
    using Real64  = double;
//    using Real128 = long double;
    
    using Complex32  = std::complex<Real32>;
    using Complex64  = std::complex<Real64>;
//    using Complex128 = std::complex<Real128>;
    
    using std::conj;
    using std::real;
    using std::imag;
    
    namespace Scalar
    {
        
        enum class Flag : char
        {
            Generic =  2,
            Plus    =  1,
            Minus   = -1,
            Zero    =  0
        };
        
        template<typename T>
        [[nodiscard]] constexpr Scalar::Flag ToFlag( const T x )
        {
            if( x == T(0) )
            {
                return Scalar::Flag::Zero;
            }
            else if( x == T(1) )
            {
                return Scalar::Flag::Plus;
            }
            else if( x == T(-1) )
            {
                return Scalar::Flag::Minus;
            }
            else
            {
                return Scalar::Flag::Generic;
            }
        }
        
    } // namespace Scalar
    
    

    
    [[nodiscard]] std::string ToString( const Scalar::Flag flag )
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

    namespace Scalar
    {    
        template<> constexpr bool ComplexQ<Real32>     = false;
        template<> constexpr bool ComplexQ<Real64>     = false;
//        template<> constexpr bool ComplexQ<Real128>    = false;
        template<> constexpr bool ComplexQ<Complex32>  = true;
        template<> constexpr bool ComplexQ<Complex64>  = true;
//        template<> constexpr bool ComplexQ<Complex128> = true;

        
        template<> constexpr bool RealQ<Real32 >       = true;
        template<> constexpr bool RealQ<Real64 >       = true;
//        template<> constexpr bool RealQ<Real128>       = true;
        template<> constexpr bool RealQ<Complex32 >    = false;
        template<> constexpr bool RealQ<Complex64 >    = false;
//        template<> constexpr bool RealQ<Complex128>    = false;
        
        template<typename T> constexpr bool ScalarQ = false;
        
        template<> constexpr bool ScalarQ<Real32 >     = true;
        template<> constexpr bool ScalarQ<Real64 >     = true;
//        template<> constexpr bool ScalarQ<Real128>     = true;
        template<> constexpr bool ScalarQ<Complex32 >  = true;
        template<> constexpr bool ScalarQ<Complex64 >  = true;
//        template<> constexpr bool ScalarQ<Complex128>  = true;
        
        template<typename T> constexpr bool FloatQ = false;
        
        template<> constexpr bool FloatQ<Real32 >     = true;
        template<> constexpr bool FloatQ<Real64 >     = true;
//        template<> constexpr bool FloatQ<Real128>     = true;
        template<> constexpr bool FloatQ<Complex32 >  = true;
        template<> constexpr bool FloatQ<Complex64 >  = true;
//        template<> constexpr bool FloatQ<Complex128>  = true;
        
        template<typename T> constexpr int Prec       = 0;
        
        template<> constexpr int Prec<Real32 >     = sizeof(Real32) * 8;
        template<> constexpr int Prec<Real64 >     = sizeof(Real64) * 8;
//        template<> constexpr int Prec<Real128>     = sizeof(Real128) * 8;
        template<> constexpr int Prec<Complex32 >  = sizeof(Real32) * 8;
        template<> constexpr int Prec<Complex64 >  = sizeof(Real64) * 8;
//        template<> constexpr int Prec<Complex128>  = sizeof(Real64) * 8;
  
        
        template<typename T>
        using Real =
            std::conditional_t<Prec<T> ==  32,  Real32,
            std::conditional_t<Prec<T> ==  64,  Real64,
//            std::conditional_t<Prec<T> == 128, Real128,
            T>>;

        template<typename T>
        using Complex =
            std::conditional_t<Prec<T> ==  32,  Complex32,
            std::conditional_t<Prec<T> ==  64,  Complex64,
//            std::conditional_t<Prec<T> == 128, Complex128,
            T>>;
        
        template<typename T> constexpr Scalar::Real<T> Zero  {0};
        template<typename T> constexpr Scalar::Real<T> One   {1};
        template<typename T> constexpr Scalar::Real<T> Two   {2};
        template<typename T> constexpr Scalar::Real<T> Three {3};
        template<typename T> constexpr Scalar::Real<T> Four  {4};
        template<typename T> constexpr Scalar::Real<T> Fife  {5};
        template<typename T> constexpr Scalar::Real<T> Six   {6};
        template<typename T> constexpr Scalar::Real<T> Half    = One<T>/Two<T>;
        template<typename T> constexpr Scalar::Real<T> Third   = One<T>/Three<T>;
        template<typename T> constexpr Scalar::Real<T> Quarter = One<T>/Four<T>;
        template<typename T> constexpr Scalar::Real<T> Fourth  = One<T>/Four<T>;
        template<typename T> constexpr Scalar::Real<T> Fifth   = One<T>/Fife<T>;
        template<typename T> constexpr Scalar::Real<T> Sixth   = One<T>/Six<T>;
        
        
        template<typename T> constexpr Scalar::Complex<T> I {0,1};
        
        template<typename T> constexpr Scalar::Real<T> Pi = static_cast<Scalar::Real<T>>(3.141592653589793238462643383279502884197);
        
        template<typename T> constexpr Scalar::Real<T> TwoPi    = Scalar::Two<T> * Scalar::Pi<T>;
        template<typename T> constexpr Scalar::Real<T> TwoPiInv = Scalar::One<T> / ( Scalar::Two<T> * Scalar::Pi<T> );
        
        template<typename T> constexpr Scalar::Real<T> eps      = std::numeric_limits<Scalar::Real<T>>::epsilon();
        template<typename T> constexpr Scalar::Real<T> Max      = std::numeric_limits<Scalar::Real<T>>::max();
        template<typename T> constexpr Scalar::Real<T> Infty    = std::numeric_limits<Scalar::Real<T>>::has_infinity ? std::numeric_limits<Scalar::Real<T>>::infinity() : std::numeric_limits<Scalar::Real<T>>::max();
        template<typename T> constexpr Scalar::Real<T> Min      = std::numeric_limits<Scalar::Real<T>>::lowest();
        
    } // namespace Scalar
    
    template<typename T>
    force_inline constexpr T Conj( const T & z )
    {
        if constexpr ( Scalar::ComplexQ<T> )
        {
            return std::conj(z) ;
        }
        else
        {
            return z;
        }
    }
    
    
    template<typename T>
    force_inline constexpr std::complex<T> Conj( const std::complex<T> & z )
    {
        return std::conj(z);
    }
    
    template<typename T>
    force_inline constexpr Scalar::Real<T> Re( const T & z )
    {
        if constexpr ( Scalar::ComplexQ<T> )
        {
            return std::real(z) ;
        }
        else
        {
            return z;
        }
    }
    
    template<typename T>
    force_inline constexpr Scalar::Real<T> Im( const T & z )
    {
        if constexpr ( Scalar::ComplexQ<T> )
        {
            return std::imag(z) ;
        }
        else
        {
            return static_cast<Scalar::Real<T>>(0);
        }
    }
    
    template<typename T>
    force_inline constexpr Scalar::Real<T> AbsSquared( const T & z )
    {
        if constexpr ( Scalar::ComplexQ<T> )
        {
            return Re(z) * Re(z) + Im(z) * Im(z) ;
        }
        else
        {
            return z * z;
        }
    }
    
    template<typename T>
    force_inline constexpr Scalar::Real<T> Abs( const T & z )
    {
        return std::abs(z);
    }
    
    
    // scalar_cast<T>(x) cast x to the precision of T, but preserves Real/Complex
    template<typename T, typename S>
    force_inline constexpr
    typename std::enable_if_t<
        Scalar::ScalarQ<T>
        &&
        Scalar::RealQ<S>,
        typename Scalar::Real<T>
    >
    scalar_cast( S x )
    {
        return static_cast<typename Scalar::Real<T>>(x);
    }

    template<typename T, typename S>
    force_inline constexpr
    typename std::enable_if_t<
        Scalar::ScalarQ<T>
        &&
        Scalar::ComplexQ<S>,
        typename Scalar::Complex<T>
    >
    scalar_cast( const S & x )
    {
        return static_cast<typename Scalar::Complex<T>>(x);
    }
    
    // First converts to Real<R> and then compute the reciprocal.
    template<typename S, typename R = S>
    force_inline constexpr R Inv( const S & a )
    {
        return Scalar::One<R> / scalar_cast<R>(a);
    }

    template<typename S, typename T, typename R = decltype( S(1)*T(1) )>
    force_inline constexpr R Frac( const S & a, const T & b )
    {
        return scalar_cast<R>(a) / scalar_cast<R>(b);
    }
    
    
    template<Scalar::Flag a_flag, typename S, typename T, typename R = decltype( S(1) * T(1) ) >
    force_inline constexpr R Mult( const S a, const T x )
    {
        if constexpr ( a_flag == Scalar::Flag::Generic )
        {
            return a * x;
        }
        else if constexpr ( a_flag == Scalar::Flag::Plus )
        {
            return x;
        }
        else if constexpr ( a_flag == Scalar::Flag::Minus )
        {
            return -x;
        }
        else if constexpr ( a_flag == Scalar::Flag::Zero )
        {
            return static_cast<R>(0);
        }
    };

    
//    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T, but preserves Real/Complex
//    template<typename S, typename T, typename R>
//    constexpr force_inline
//    std::enable_if_t<
//        Scalar::ScalarQ<S> && Scalar::ScalarQ<T>
//        &&
//        Scalar::RealQ<R>
//        &&
//        std::less_equal(Scalar::Prec<S>,Scalar::Prec<T>),
//        typename Scalar::Real<S>
//    >
//    lo_prec_cast( R x )
//    {
//        return static_cast<typename Scalar::Real<S>>(x);
//    }
//
//    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T
//    template<typename S, typename T, typename R>
//    constexpr force_inline
//    std::enable_if_t<
//        Scalar::ScalarQ<S> && Scalar::ScalarQ<T>
//        &&
//        Scalar::RealQ<R>
//        &&
//        std::greater(Scalar::Prec<S>, Scalar::Prec<T>),
//        typename Scalar::Real<T>
//    >
//    lo_prec_cast( R x )
//    {
//        return static_cast<typename Scalar::Real<T>>(x);
//    }
//
//
//
//    // hi_prec_cast<S,T>(x) casts x to the higher precision of S and T, but preserves Real/Complex
//    template<typename S, typename T, typename R>
//    constexpr force_inline
//    std::enable_if_t<
//        Scalar::ScalarQ<S> && Scalar::ScalarQ<T>
//        &&
//        Scalar::RealQ<R>
//        &&
//        std::greater_equal(Scalar::Prec<S>,Scalar::Prec<T>),
//        typename Scalar::Real<S>
//    >
//    hi_prec_cast( R x )
//    {
//        return static_cast<typename Scalar::Real<S>>(x);
//    }
//
//    // lo_prec_cast<S,T>(x) casts x to the lower precision of S and T
//    template<typename S, typename T, typename R>
//    constexpr force_inline
//    std::enable_if_t<
//        Scalar::ScalarQ<S> && Scalar::ScalarQ<T>
//        &&
//        Scalar::RealQ<R>
//        &&
//        std::less(Scalar::Prec<S>, Scalar::Prec<T>),
//        typename Scalar::Real<T>
//    >
//    hi_prec_cast( R x )
//    {
//        return static_cast<typename Scalar::Real<T>>(x);
//
//    }
    
    template<typename Scal>
    inline constexpr bool NaNQ( const Scal & x )
    {
        // This is a work-around to detect NaNs under -ffast-math option.
        // Not sure whether this is portable.
        if constexpr ( Scalar::RealQ<Scal> )
        {
//            return !(
//                (std::numeric_limits<Scal>::lowest() <= x)
//                &&
//                ( x <=  std::numeric_limits<Scal>::max() )
//            );
            
            // This works at least for Apple clang.
            return (x < std::numeric_limits<Scal>::lowest());
        }
        else if constexpr ( Scalar::ComplexQ<Scal> )
        {
            return NaNQ( Re(x) ) || NaNQ( Im(x) );
        }
        else
        {
            return true;
        }
    }
    
#define ASSERT_REAL(R) static_assert( Scalar::RealQ<R>, "Template parameter " #R " must be a real-valued type." );
    
#define ASSERT_COMPLEX(C) static_assert( Scalar::ComplexQ<C>, "Template parameter " #C " must be a complex-valued type." );
}
