namespace Tools
{
    template<typename Real, typename T>
    typename std::enable_if_t<!std::is_integral_v<T>,Real>
    inline Power( const Real base, const T exponent )
    {
        // Warning: Use only for positive base! This is basically pow with certain checks and cases deactivated
        return (base > Scalar::Zero<Real>) ? std::exp2( static_cast<Real>(exponent) * std::log2(base) ) : ( static_cast<Real>(exponent)!=Scalar::Zero<Real> ? Scalar::Zero<Real> : Scalar::One<Real> );
    } // Power
    
    template<typename Real, typename Int>
    typename std::enable_if_t<std::is_integral_v<Int>,Real>
    inline Power( const Real base, const Int exponent)
    {
        if( exponent >= 0)
        {
            switch( exponent )
            {
                case 0:
                {
                    return Scalar::One<Real>;
                }
                case 1:
                {
                    return base;
                }
                case 2:
                {
                    return base * base;
                }
                case 3:
                {
                    return base * base * base;
                }
                case 4:
                {
                    Real b2 = base * base;
                    return b2 * b2;
                }
                case 5:
                {
                    Real b2 = base * base;
                    return b2 * b2 * base;
                }
                case 6:
                {
                    Real b2 = base * base;
                    return b2 * b2 * b2;
                }
                case 7:
                {
                    Real b2 = base * base;
                    Real b4 = b2 * b2;
                    return b4 * b2 * base;
                }
                case 8:
                {
                    Real b2 = base * base;
                    Real b4 = b2 * b2;
                    return b4 * b4;
                }
                case 9:
                {
                    Real b2 = base * base;
                    Real b4 = b2 * b2;
                    return b4 * b4 * base;
                }
                case 10:
                {
                    Real b2 = base * base;
                    Real b4 = b2 * b2;
                    return b4 * b4 * b2;
                }
                case 11:
                {
                    Real b2 = base * base;
                    Real b4 = b2 * b2;
                    return b4 * b4 * b2 * base;
                }
                case 12:
                {
                    Real b2 = base * base;
                    Real b4 = b2 * b2;
                    return b4 * b4 * b4;
                }

                default:
                {
                    Real exp = exponent;
                    return Power(base, exp);
                }
            }
        }
        else
        {
            return Inv( Power(base, -exponent) );
        }
    } // Power
    
    template<typename T>
    inline T Sign( const T x)
    {
        constexpr T zero = 0;
        
        return static_cast<T>( (zero < x) - (x < zero) );
    }
    
    template<typename T>
    inline int IntSign( const T x)
    {
        constexpr T zero = 0;
        
        return static_cast<int>( (zero < x) - (x < zero) );
    }
    
    
    
    
    template<typename T>
    force_inline constexpr bool Equal3( const T & a, const T & b, const T & c )
    {
        return (a == b) && (b == c);
    }
    
    template<typename Real>
    force_inline constexpr Real Max( const Real & x, const Real & y )
    {
        if constexpr ( std::is_floating_point_v<Real> )
        {
            return std::fmax(x,y);
        }
        else
        {
            return std::max(x,y);
        }
    }
    
    template<typename Real>
    force_inline constexpr Real Min( const Real & x, const Real & y )
    {
        if constexpr ( std::is_floating_point_v<Real> )
        {
            return std::fmin(x,y);
        }
        else
        {
            return std::min(x,y);
        }
    }
    
    
    template<typename Real>
    force_inline constexpr std::pair<Real,Real> MinMax( const Real & x, const Real & y )
    {
        if constexpr ( std::is_floating_point_v<Real> )
        {
            return std::pair<Real,Real>( std::fmin(x,y), std::fmax(x,y) );
        }
        else
        {
            return std::minmax(x,y);
        }
    }
    
    template<typename Real>
    force_inline constexpr Real Ramp( const Real & x )
    {
        static_assert(Scalar::RealQ<Real>, "Argument must be of a real type.");
        return Max( Scalar::Zero<Real>, x );
    }
    
    template<typename Real>
    force_inline constexpr Real Clamp( const Real & x, const Real & a, const Real & b )
    {
        static_assert(Scalar::RealQ<Real>, "Argument must be of a real type.");
        return Max( a, Min( b, x ) );
    }
    
    template<typename Real>
    force_inline constexpr Real Ramp_1( const Real & x )
    {
        static_assert(Scalar::RealQ<Real>, "Argument must be of a real type.");
        return Max( Scalar::One<Real>, x );
    }
    
    template<typename Real>
    force_inline constexpr Real Sqrt( const Real & x )
    {
        return std::sqrt(x);
    }
    
    template<typename Real>
    force_inline constexpr Real InvSqrt( const Real & x )
    {
        return Inv( Sqrt(x) );
    }
    
    template<int AmbDim, typename Real>
    inline Real AngleBetweenUnitVectors( cptr<Real> x, cptr<Real> y )
    {
        Real a = Scalar::Zero<Real>;
        Real b = Scalar::Zero<Real>;
        
        for( int i = 0; i < AmbDim; ++i )
        {
            a += (x[i]-y[i]) * (x[i]-y[i]);
            b += (x[i]+y[i]) * (x[i]+y[i]);
        }
        
        return Scalar::Two<Real> * atan( std::sqrt(a/b) );
    }
    
    template<int AmbDim, typename Real>
    inline Real Angle( cptr<Real> x, cptr<Real> y )
    {
        Real a2 = Scalar::Zero<Real>;
        Real b2 = Scalar::Zero<Real>;
        
        Real u [AmbDim] = {};
        Real v [AmbDim] = {};
        
        for( int i = 0; i < AmbDim; ++i )
        {
            a2 += x[i] * x[i];
            b2 += y[i] * y[i];
        }
        
        const Real a_inv = InvSqrt(a2);
        const Real b_inv = InvSqrt(b2);
        
        for( int i = 0; i < AmbDim; ++i )
        {
            u[i] = x[i] * a_inv;
            v[i] = v[i] * a_inv;
        }
        
        return AngleBetweenUnitVectors<AmbDim>(&u[0],&v[0]);
    }
    

//    template<typename T>
//    inline static constexpr T Factorial( T n )
//    {
//        T result ( 1 );
//        
//        for( T k = static_cast<T>(2); k <= n; ++k )
//        {
//            result *= k;
//        }
//        
//        return result;
//    }
    
    template<typename T>
    inline static constexpr T Factorial( T n )
    {
        T result ( 1 );
        
        for( T k = n; k > 1; k -= 1 )
        {
            result *= k;
        }
        
        return result;
    }
    
    template<typename T>
    inline static constexpr T DoubleFactorial( T n )
    {
        T result ( 1 );
        
        for( T k = n; k > 1; k -= 2 )
        {
            result *= k;
        }
        
        return result;
    }
    
    
    template<typename T_out, typename T_in>
    inline static constexpr T_out Delta( const T_in & a, const T_in & b )
    {
        return static_cast<T_out>( a == b );
    }
    
    

    namespace Detail
    {
        template<typename Real>
        Real constexpr SqrtNewtonRaphson( const Real x, const Real curr, const Real prev )
        {
            ASSERT_REAL(Real)
            
            return curr == prev
                ? curr
                : SqrtNewtonRaphson(x, Scalar::Half<Real> * (curr + x / curr), curr);
        }
    }

    /*
    * Constexpr version of the square root
    * Return value:
    *   - For a finite and non-negative value of "x", returns an approximation for the square root of "x"
    *   - Otherwise, returns NaN
    * See https://stackoverflow.com/a/34134071/8248900.
    */
    template<typename Real>
    Real constexpr cSqrt( const Real x)
    {
        ASSERT_REAL(Real)
        
        return (x >= Scalar::Zero<Real> ) && (x < std::numeric_limits<Real>::infinity())
            ? Detail::SqrtNewtonRaphson(x, x, Scalar::Zero<Real> )
            : std::numeric_limits<Real>::quiet_NaN();
    }
    
    
    
    template<typename S, typename T>
    static constexpr bool SameQ = std::is_same_v<S,T>;
    
    template<typename T>
    static constexpr bool VectorizableQ = vec_enabledQ && ( SameQ<T,Real32> || SameQ<T,Real64> || SameQ<T,Int16> ||SameQ<T,Int32> || SameQ<T,Int64> || SameQ<T,UInt16> ||SameQ<T,UInt32> || SameQ<T,UInt64> );
    
    
    template < typename T >
    inline static constexpr T SphereVolume ( const Size_T n )
    {
        
        T result (  (n % 2 == 0) ? Scalar::Two < T > : Scalar::TwoPi < T > );
        
        for ( T k = n; k > Scalar::One < T >; k -= Scalar::Two < T > )
        {
            
            result *= Frac < T > ( Scalar::TwoPi < T >, k - 1);
        }
        
        return result;
    }
    
    template < typename T >
    inline static constexpr T BallVolume ( const Size_T n )
    {
        
        T result ( (n % 2 == 0) ? Scalar::One < T > : Scalar::Two < T > );
        
        for ( T k = n; k > Scalar::One < T >; k -= Scalar::Two < T > )
        {
            
            result *= Frac < T > ( Scalar::TwoPi < T >, k );
        }
        
        return result;
    }
    
    template < typename T >
    inline static constexpr T SOVolume ( const Size_T n )
    {
        
        T result ( Scalar::One < T > );
        
        for ( Size_T k = 1; k < n; ++k )
        {
            
            result *= SphereVolume<T>(k);
        }
        
        return result;
    }
    
    
    template < typename T >
    inline static constexpr T GammaQuotient ( const T x, const T a )
    {
        // Computes Gamma(x+a)/Gamma(x) with considerably less risk of overflow than std::tgamma(x+a)/std::tgamma(x).

        ASSERT_FLOAT(T);
        
        static_assert( Scalar::RealQ<T>, "GammaQuotient is only implemented for real floating point types." );
        
        if( (x > Scalar::Zero<T>) && (a >= Scalar::Zero<T>) )
        {
            if( x <= Scalar::One<T> )
            {
                return std::tgamma(x+a) / std::tgamma(x);
            }
            else
            {
                T n;
                
                T r = modf( x, &n ) + Scalar::One<T>;
                
                // Adding 1 to guarantee that r > 0.
                
                // So r lies within [1,2).
                
                T result = std::tgamma(r+a) / std::tgamma(r);
                
                for ( T y = x; y --> r; )
                {
                    result += (a / y) * result;
                }
                
                return result;
            }
        }
        else
        {
            wprint("GammaQuotient is only implemented for positive x and nonnegative a. x = " + ToString(x) + ", a = " + ToString(a) + "." );
            return 1;
        }
    }
}

 
