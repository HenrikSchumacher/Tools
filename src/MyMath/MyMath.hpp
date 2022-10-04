namespace MyMath
{
    
    template<typename Real, typename T>
    std::enable_if_t<!std::is_integral_v<T>,Real> inline pow( const Real base, const T exponent )
    {
        // Warning: Use only for positive base! This is basically pow with certain checks and cases deactivated
        return base > static_cast<Real>(0) ? std::exp2( static_cast<Real>(exponent) * std::log2(base) ) : ( static_cast<Real>(exponent)!=static_cast<Real>(0) ? static_cast<Real>(0) : static_cast<Real>(1) );
    } // pow
    
    template<typename Real, typename Int>
    std::enable_if_t<std::is_integral_v<Int>,Real> inline pow( const Real base, const Int exponent)
    {
        if( exponent >= 0)
        {
            switch( exponent )
            {
                case 0:
                {
                    return static_cast<Real>(1);
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
                    return pow(base, exp);
                }
            }
        }
        else
        {
            return static_cast<Real>(1)/pow(base, -exponent);
        }
    } // pow
    
    template<typename T>
    inline T sign( const T val)
    {
        return static_cast<T>( (static_cast<T>(0) < val) - (val < static_cast<T>(0)) );
    }
    
    template<int AmbDim, typename Real>
    inline Real AngleBetweenUnitVectors( const Real * const restrict x, const Real * const restrict y )
    {
        Real a = static_cast<Real>(0);
        Real b = static_cast<Real>(0);
        
        for( int i = 0; i < AmbDim; ++i )
        {
            a += (x[i]-y[i]) * (x[i]-y[i]);
            b += (x[i]+y[i]) * (x[i]+y[i]);
        }
        
        return 2*atan( std::sqrt(a/b) );
    }
    
    template<int AmbDim, typename Real>
    inline Real Angle( const Real * const restrict x, const Real * const restrict y )
    {
        Real a2 = static_cast<Real>(0);
        Real b2 = static_cast<Real>(0);
        
        Real u [AmbDim] = {};
        Real v [AmbDim] = {};
        
        for( int i = 0; i < AmbDim; ++i )
        {
            a2 += x[i] * x[i];
            b2 += y[i] * y[i];
        }
        
        const Real a_inv = static_cast<Real>(1)/std::sqrt(a2);
        const Real b_inv = static_cast<Real>(1)/std::sqrt(b2);
        
        for( int i = 0; i < AmbDim; ++i )
        {
            u[i] = x[i] * a_inv;
            v[i] = v[i] * a_inv;
        }
        
        return AngleBetweenUnitVectors<AmbDim>(&u[0],&v[0]);
    }
    


    
    
    
    
    
    template<typename Real>
    void RealCubicSolve(
        const Real A, const Real B, const Real C, const Real D, Real * restrict t_out
    )
    {
        constexpr Real zero  = 0;
        constexpr Real one   = 1;
        constexpr Real two   = 2;
        constexpr Real three = 3;
        
        constexpr Real factor = static_cast<Real>(1)/static_cast<Real>(27);
        constexpr Real third  = static_cast<Real>(1)/static_cast<Real>(3);
        constexpr Real half   = static_cast<Real>(1)/static_cast<Real>(2);

        
        constexpr Real Pi_third = static_cast<Real>(PI) * third;
        constexpr Real Pi_sixth = static_cast<Real>(PI) * third * half;
        
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
                const Real disc = p * p - static_cast<Real>(4)*q;
                
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
            
            const Real disc = q * q * static_cast<Real>(0.25) + p * p * p * factor;
            if( std::abs(disc) > eps )
            {
                if( disc > zero )
                {
                    // std::abs(disc) > eps
                    
//                    print("exactly one real solutions");
                    
                    const Real sqrt_disc = std::sqrt(disc);
                    
                    const Real r1 = -q * half + sqrt_disc;
                    const Real r2 = -q * half - sqrt_disc;
                    
                    x[0] = ((r1>=zero) ? one : -one) * std::pow( std::abs(r1), third)
                         +
                           ((r2>=zero) ? one : -one) * std::pow( std::abs(r2), third)
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

}

 
