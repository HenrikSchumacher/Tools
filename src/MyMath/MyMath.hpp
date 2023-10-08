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
    inline T Sign( const T val)
    {
        return static_cast<T>( (static_cast<T>(0) < val) - (val < static_cast<T>(0)) ) ;
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
        
        return 2*atan( std::sqrt(a/b) );
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
    
    template<typename Real>
    void RealCubicSolve(
        const Real A, const Real B, const Real C, const Real D, mptr<Real> t_out
    )
    {
        static_assert( Scalar::RealQ<Real>, "Input parameters must be real floating point types.");
        
        constexpr Real zero  = 0;
        constexpr Real one   = 1;
        constexpr Real two   = 2;
        constexpr Real three = 3;
        
        constexpr Real factor = Inv<Real>(27);
        constexpr Real third  = Inv<Real>(3);
        constexpr Real half   = Inv<Real>(2);

        
        constexpr Real Pi_third = Scalar::Pi<Real> * third;
        constexpr Real Pi_sixth = Scalar::Pi<Real> * third * half;
        
        constexpr Real sqrt_three          = 1.7320508075688772;
        constexpr Real two_over_sqrt_three = 1.1547005383792517;
        constexpr Real factor_1 = three * sqrt_three * half;
        
        static constexpr Real eps   = 100 * std::numeric_limits<Real>::epsilon();
        static constexpr Real infty = std::numeric_limits<Real>::max();
        
        Real x[3] = {};
        
        if( std::abs(A) < eps )
        {
            if( std::abs(B) < eps )
            {
                if( std::abs(C) < eps )
                {
//                    print("constant equation");
                    if( std::abs(D) < eps )
                    {
                        x[0] = zero;
                        x[1] = infty;
                        x[2] = infty;
                    }
                    else
                    {
                        x[0] = infty;
                        x[1] = infty;
                        x[2] = infty;
                    }
                }
                else
                {
//                    print("linear equation");
                    
                    x[0] = -D/C;
                    x[1] = infty;
                    x[2] = infty;
                }
            }
            else
            {
//                print("quadratic equation");
                
                const Real p = C/B;
                const Real q = D/B;
                const Real disc = p * p - Scalar::Four<Real> * q;
                
                if( std::abs(disc) < eps )
                {
//                    print("no real solution");
                    
                    x[0] = infty;
                    x[1] = infty;
                    x[2] = infty;
                }
                else
                {
//                    print("two real solutions");
                    const Real sqrt_disc = std::sqrt(disc);
                    x[0] = half + sqrt_disc;
                    x[1] = half - sqrt_disc;
                    x[2] = infty;
                }
            }
        }
        else
        {
//            print("cubic equation");
            
            const Real a = B/A;
            const Real b = C/A;
            const Real c = D/A;
            const Real aa = a * a;
            const Real p = b - aa * third;
            const Real q = two * aa* a * factor - a * b * third + c;
            
            const Real disc = q * q * Scalar::Quarter<Real> + p * p * p * factor;
            if( std::abs(disc) > eps )
            {
                if( disc > zero )
                {
                    // std::abs(disc) > eps
                    
//                    print("exactly one real solutions");
                    
                    const Real sqrt_disc = std::sqrt(disc);
                    
                    const Real r1 = -q * half + sqrt_disc;
                    const Real r2 = -q * half - sqrt_disc;
                    
                    x[0] = ((r1>=zero) ? one : -one) * std::pow( std::abs(r1), third )
                         +
                           ((r2>=zero) ? one : -one) * std::pow( std::abs(r2), third )
                         -
                         a * third;
                    x[1] = infty;
                    x[2] = infty;

                }
                else
                {
                    // std::abs(disc) < eps

//                    print("exactly three real solutions");
                    
                    const Real sqrt_p = std::sqrt(-p);
                    
                    const Real theta_third = std::asin( factor_1 * q / ( (-p) * sqrt_p ) ) * third;
                    
                    const Real a_third = a * third;
                    
                    x[0] =  two_over_sqrt_three * sqrt_p * std::sin(theta_third)            - a_third;
                    x[1] = -two_over_sqrt_three * sqrt_p * std::sin(theta_third + Pi_third) - a_third;
                    x[2] =  two_over_sqrt_three * sqrt_p * std::cos(theta_third + Pi_sixth) - a_third;
                }
            }
            else
            {
//                print("exactly two real solutions");
                
                const Real power = std::pow( half * q, third );
                const Real a_third = a * third;
                
                x[0] = - two * power - a_third;
                x[1] = power - a_third;
                x[2] = infty;
            }
        }
        
//        // check
//        for( int i = 0; i < 3; ++i )
//        {
//            if( x[i] < infty )
//            {
//                valprint("f(x["+ToString(i)+"])",
//                         A * x[i] * x[i] * x[i] + B * x[i] * x[i] + C * x[i] + D
//                );
//            }
//        }
        
        t_out[0] = x[0];
        t_out[1] = x[1];
        t_out[2] = x[2];
        
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
    
    
    
    
    
    template<typename T>
    force_inline constexpr bool Equal3( const T & a, const T & b, const T & c )
    {
        return (a == b) && (b == c);
    }
    
    template<typename Real>
    force_inline constexpr Real Max( const Real & x, const Real & y )
    {
        return std::max( x, y );
    }
    
    template<typename Real>
    force_inline constexpr Real Min( const Real & x, const Real & y )
    {
        return std::min( x, y );
    }
    
    template<typename Real>
    force_inline constexpr Real Ramp( const Real & x )
    {
        return std::max( Scalar::Zero<Real>, x );
    }
    
    template<typename Real>
    force_inline constexpr Real Sqrt( const Real & x )
    {
        return std::sqrt( x );
    }
    
    template<typename Real>
    force_inline constexpr Real InvSqrt( const Real & x )
    {
        return Inv( Sqrt(x) );
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
            wprint("GammaQuotient is only implemented for positive x and nonnegative a.");
            return 1;
        }
    }
}

 
