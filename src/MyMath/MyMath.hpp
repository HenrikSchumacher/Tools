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
    
  
    template<typename Real>
    void RealCubicSolve( const Real b, const Real c, const Real d, mptr<Real> x,
        const Real tol = 16 * Scalar::eps<Real>, const int max_iter = 16
    )
    {
        // Computes the real roots of the cubic equation x^3 + b * x^2 + c * x + d = 0.
        
        static_assert( Scalar::RealQ<Real>, "Input parameters must be of real floating point types.");
        
        constexpr Real c_0   = 0;
        constexpr Real c_3   = 3;

        constexpr Real half  = Inv<Real>(2);
        
        constexpr Real C_0 = Frac<Real>(2,27);
        constexpr Real C_1 = Frac<Real>(1,3);
        
        static constexpr Real infty = std::numeric_limits<Real>::max();
        
        // Convert to depressed form:
        // t = x - shift
        // a * x^3 + b * x^2 + c * x + d = t^3 + p * t + q
        
        const Real bb = b * b;
        
        const Real p  = c - bb * C_1;
        
        const Real shift = - b * C_1;
        
        const Real q  = (C_0 * bb - c * C_1) * b + d;

        // Now we apply the iterative method from
        // https://de.wikipedia.org/wiki/Kubische_Gleichung#Schnelle_numerische_Berechnung
        
        if( p == c_0 )
        {
            // Three-fold root.
            
            const Real t = - std::cbrt( q );
            
            x[0] = t + shift;
            x[1] = t + shift;
            x[2] = t + shift;
            
            return;
        }
        
        // Halley / Newton iteration of upper bound
        auto H_delta = [p,q]()
        {
            constexpr Real C_0 = static_cast<Real>(40) * cSqrt( c_3 );
            constexpr Real C_1 = static_cast<Real>(18) * cSqrt( c_3 );
            
            const Real sqrt_minus_p = Sqrt( - p );
            
            return Frac<Real>(
                p * ( C_0 * sqrt_minus_p * p + static_cast<Real>(63) * q ),
                static_cast<Real>(69) * p * p - C_1 * sqrt_minus_p * q
            );
        };
        
        auto H_minus_delta = [p,q]()
        {
            constexpr Real C_0 = static_cast<Real>(40) * cSqrt( c_3 );
            constexpr Real C_1 = static_cast<Real>(18) * cSqrt( c_3 );
            
            const Real sqrt_minus_p = Sqrt( - p );
            
            return Frac<Real>(
                p * ( - C_0 * sqrt_minus_p * p + static_cast<Real>(63) * q ),
                static_cast<Real>(69) * p * p + C_1 * sqrt_minus_p * q
            );
        };
        
        // General Halley iteration.
        auto Step = [p,q]( const Real t )
        {
            const Real tt = t * t;
            
            const Real f        = (tt + p) * t + q;
            const Real Df       = c_3 * tt + p;
            const Real DDf_half = c_3 * t;
            
            return ( Df * f ) / ( Df * Df - DDf_half * f);
        };
        
//        // General Newton iteration.
//        auto Step = [p,q]( const Real t )
//        {
//            const Real tt = t * t;
//            
//            const Real f        = (tt + p) * t + q;
//            const Real Df       = c_3 * tt + p;
//            
//            return f / Df ;
//        };
        
        
        Real t_0 = 0;
        
        if( q != c_0 )
        {
            // First Halley iteration.
            Real t  = (p > c_0 ) ? -q/p : ( (q > c_0 ) ? H_minus_delta() : H_delta() );
            
            Real delta_t = Step(t);
            t -= delta_t;
            
            int iter = 2;
            
            while( (Abs(delta_t) > tol) && (iter < max_iter) )
            {
                ++iter;
                
                delta_t = Step(t);
                t -= delta_t;
            }
            
            if( iter >= max_iter )
            {
                const Real f = (t * t + p) * t + q;
                
                eprint( std::string("RealCubicSolve( ") + ToString(b) + ", " + ToString(c) + ", " + ToString(d) + "," + ToString(tol) + ", " + ToString(max_iter) + " ) did not converge. Residual = " + ToString(f) + ", last step size = " + ToString(delta_t) + ".");
            }

            t_0 = t;
        }
        
        x[0] = t_0 + shift;
        
        const Real discriminant = - Scalar::Four<Real> * p - c_3 * t_0 * t_0;
        
        if( discriminant >= c_0 )
        {
            const Real sqrt_discriminant = Sqrt(discriminant);
            
            x[1] = shift - half * ( t_0 + sqrt_discriminant);
            x[2] = shift - half * ( t_0 - sqrt_discriminant);
        }
        else
        {
            x[1] = infty;
            x[2] = infty;
        }
    }
    
    
    
    template<typename Real>
    void RealCubicSolve( const Real a, const Real b, const Real c, const Real d, mptr<Real> x,
        const Real tol = 16 * Scalar::eps<Real>, const int max_iter = 16
    )
    {
        // Computes the real roots of the cubic equation a * x^3 + b * x^2 + c * x + d = 0.
        
        static_assert( Scalar::RealQ<Real>, "Input parameters must be of real floating point types.");
        
        constexpr Real half = Inv<Real>(2);
        
        static constexpr Real eps   = 8 * std::numeric_limits<Real>::epsilon();
        static constexpr Real infty = std::numeric_limits<Real>::max();
        
        if( std::abs(a) < eps )
        {
            if( std::abs(b) < eps )
            {
                if( std::abs(c) < eps )
                {
//                    print("constant equation");
                    
                    if( std::abs(d) < eps )
                    {
                        x[0] = Scalar::Zero<Real>;
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
                    
                    x[0] = -d/c;
                    x[1] = infty;
                    x[2] = infty;
                }
            }
            else
            {
//                print("quadratic equation");
                
                const Real p = c/b;
                const Real q = d/b;
                const Real discriminant = p * p - Scalar::Four<Real> * q;
                
                if( std::abs(discriminant) < eps )
                {
//                    print("no real solution");
                    
                    x[0] = infty;
                    x[1] = infty;
                    x[2] = infty;
                }
                else
                {
//                    print("two real solutions");
                    const Real sqrt_discriminant = Sqrt(discriminant);
                    x[0] = half + sqrt_discriminant;
                    x[1] = half - sqrt_discriminant;
                    x[2] = infty;
                }
            }
        }
        else
        {
//            print("cubic equation");

            const Real a_inv = Inv(a);
            
            RealCubicSolve( b * a_inv, c * a_inv, d * a_inv, x, tol, max_iter );
        }
        
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

 
