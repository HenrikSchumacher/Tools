namespace Tools
{
    
    template<typename Real>
    int RealCubicSolve_UnitInterval_RegulaFalsi(
        const Real c_3, const Real c_2, const Real c_1, const Real c_0,
        mptr<Real> x_out,
        const Real tol = 64 * Scalar::eps<Real>, const int max_iter = 64
    )
    {
        constexpr Real a = 0;
        constexpr Real b = 1;
        
        constexpr Real zero = 0;
        
        // Computes the real roots of the cubic equation c_3 * x^3 + c_2 * x^2 + c_1 * x + c_0 = 0
        // in the interval [a,b]. We assume a < b.
        
        const Real TOL = tol * Abs(b-a);
        
        static_assert( Scalar::RealQ<Real>, "Input parameters must be of real floating point types.");
                
        static constexpr Real infty = std::numeric_limits<Real>::max();
        
        auto f = [c_3,c_2,c_1,c_0]( const Real x )
        {
            return ((c_3 * x + c_2) * x + c_1) * x + c_0;
        };
        
        const Real Dc_3  = Scalar::Three<Real> * c_3;
        const Real Dc_2  = Scalar::Two<Real>   * c_2;
        
        auto Df = [Dc_3,Dc_2,c_1]( const Real x )
        {
            return (( Dc_3 * x + Dc_2) * x + c_1);
        };
        
        const Real x_I = - c_2 / ( Scalar::Three<Real> * c_3);
        const Real D   = c_2 * c_2 - Scalar::Three<Real> * c_1 * c_3;
        
        auto Solve = [f,TOL]( const Real L, const Real R, const Real x_init )
        {
            return RegulaFalsi<0>( f, L, R, 0., TOL );
        };

        std::vector<Real> cand;
        cand.reserve(5);
        std::vector<Real> sign;
        sign.reserve(5);
        
        
        cand.push_back(a);
        sign.push_back(Sign(c_0));
        
        if( D > zero )
        {
            const Real delta = Sqrt(D) /  ( Scalar::Three<Real> * c_3);
            
            // The two critical points.
            const Real x_L = x_I - delta;
            const Real x_R = x_I + delta;
            
            if( a < x_L && x_L < b )
            {
                cand.push_back(x_L);
                sign.push_back(Sign(f(x_L)));
            }
            
            if( a < x_I && x_I < b )
            {
                cand.push_back(x_I);
                sign.push_back(Sign(f(x_I)));
            }
            
            if( a < x_R && x_R < b )
            {
                cand.push_back(x_R);
                sign.push_back(Sign(f(x_R)));
            }
            
        }
        else if( D < zero )
        {
            // At most one solution, as f is strictly increasing.
        }
        else if( D == zero )
        {
            // At most one solution, but x_I is strictly monotone.
            
            if( a < x_I && x_I < b )
            {
                cand.push_back(x_I);
                sign.push_back(Sign(f(x_I)));
            }
        }
        
        cand.push_back(b);
        sign.push_back(Sign(c_0+c_1+c_2+c_3));
        
        int counter = 0;
        
        if( sign[0] == zero )
        {
            x_out[counter++] = cand[0];
        }
        
        for( Size_T i = 1; i < cand.size(); ++i )
        {
            if( sign[i-1] * sign[i] < zero )
            {
                x_out[counter++] = Solve( cand[i-1], cand[i], Scalar::Half<Real>*(cand[i-1]+cand[i]) );
            }
                
            if( sign[i] == zero )
            {
                x_out[counter++] = cand[i];
            }
        }
        
        for( int i = counter; i < 3; ++i )
        {
            x_out[i] = infty;
        }
            
        return counter;
    }
    
    template<typename Real>
    int RealCubicSolve_UnitInterval_Newton(
        const Real c_3, const Real c_2, const Real c_1, const Real c_0,
        mptr<Real> x_out,
        const Real tol = 64 * Scalar::eps<Real>, const int max_iter = 64
    )
    {
        constexpr Real a = 0;
        constexpr Real b = 1;
        
        constexpr Real zero = 0;
        
        // Computes the real roots of the cubic equation c_3 * x^3 + c_2 * x^2 + c_1 * x + c_0 = 0
        // in the interval [a,b]. We assume a < b.
        
        const Real TOL = tol * Abs(b-a);
        
        static_assert( Scalar::RealQ<Real>, "Input parameters must be of real floating point types.");
                
        static constexpr Real infty = std::numeric_limits<Real>::max();
        
        auto f = [c_3,c_2,c_1,c_0]( const Real x )
        {
            return ((c_3 * x + c_2) * x + c_1) * x + c_0;
        };
        
        const Real Dc_3  = Scalar::Three<Real> * c_3;
        const Real Dc_2  = Scalar::Two<Real>   * c_2;
        
        auto Df = [Dc_3,Dc_2,c_1]( const Real x )
        {
            return (( Dc_3 * x + Dc_2) * x + c_1);
        };
        
        const Real x_I = - c_2 / ( Scalar::Three<Real> * c_3);
        const Real D   = c_2 * c_2 - Scalar::Three<Real> * c_1 * c_3;
        
        auto Solve = [=]( const Real L, const Real R, const Real x_init )
        {
            auto Newton = [=]( const Real x )
            {
                return  Max(a,Min(b, x - f(x)/Df(x)));
            };

            Real x_old = x_init;
            Real x     = Newton(x_init);

            int iter = 1;

            while( Abs(x-x_old) > TOL && iter < max_iter)
            {
                ++iter;
                x_old = x;
                x = Newton(x);
            }

            return x;
        };
        
        std::vector<Real> cand;
        cand.reserve(5);
        std::vector<Real> sign;
        sign.reserve(5);
        
        
        cand.push_back(a);
        sign.push_back(Sign(c_0));
        
        if( D > zero )
        {
            const Real delta = Sqrt(D) /  ( Scalar::Three<Real> * c_3);
            
            // The two critical points.
            const Real x_L = x_I - delta;
            const Real x_R = x_I + delta;
            
            if( a < x_L && x_L < b )
            {
                cand.push_back(x_L);
                sign.push_back(Sign(f(x_L)));
            }
            
            if( a < x_I && x_I < b )
            {
                cand.push_back(x_I);
                sign.push_back(Sign(f(x_I)));
            }
            
            if( a < x_R && x_R < b )
            {
                cand.push_back(x_R);
                sign.push_back(Sign(f(x_R)));
            }
            
        }
        else if( D < zero )
        {
            // At most one solution, as f is strictly increasing.
        }
        else if( D == zero )
        {
            // At most one solution, but x_I is strictly monotone.
            
            if( a < x_I && x_I < b )
            {
                cand.push_back(x_I);
                sign.push_back(Sign(f(x_I)));
            }
        }
        
        cand.push_back(b);
        sign.push_back(Sign(c_0+c_1+c_2+c_3));
        
        int counter = 0;
        
        if( sign[0] == zero )
        {
            x_out[counter++] = cand[0];
        }
        
        for( Size_T i = 1; i < cand.size(); ++i )
        {
            if( sign[i-1] * sign[i] < zero )
            {
                x_out[counter++] = Solve( cand[i-1], cand[i], Scalar::Half<Real>*(cand[i-1]+cand[i]) );
            }
                
            if( sign[i] == zero )
            {
                x_out[counter++] = cand[i];
            }
        }
        
        for( int i = counter; i < 3; ++i )
        {
            x_out[i] = infty;
        }
            
        return counter;
    }
    
    
    template<typename Real>
    void RealCubicSolve_Iterative( const Real b, const Real c, const Real d, mptr<Real> x_out,
        const Real tol = 64 * Scalar::eps<Real>, const int max_iter = 64
    )
    {
        // Computes the real roots of the cubic equation x^3 + b * x^2 + c * x + d = 0.
        
        static_assert( Scalar::RealQ<Real>, "Input parameters must be of real floating point types.");
        
        constexpr Real c_0   = 0;
        constexpr Real c_2   = 2;
        constexpr Real c_3   = 3;
        
        constexpr Real C_1 = Frac<Real>(1,3);
        
        static constexpr Real infty = std::numeric_limits<Real>::max();
        
        
        // We apply the iterative method from
        // https://de.wikipedia.org/wiki/Kubische_Gleichung#Schnelle_numerische_Berechnung
        
//        dump(b);
//        dump(c);
//        dump(d);
        
        const Real x_infl = - b * C_1;
        
        const Real y_infl = ( (x_infl + b) * x_infl + c) * x_infl + d;
        
        Real x_0 = x_infl;
        
//        dump(x_infl);
        
        if( y_infl != c_0 )
        {
            const Real D = b * b - c_3 * c;
            
            
            if( D == c_0 )
            {
                x_0 = x_infl - std::cbrt(y_infl);
            }
            else
            {
                
                // Halley iteration step.
                auto Step = [b,c,d]( const Real x )
                {
                    const Real f        = ( ( x + b) * x + c) * x + d;
                    const Real Df       = ( c_3 * x + c_2 * b) * x + c;
                    const Real DDf_half = ( c_3 * x + b);
                    
                    return -( Df * f ) / ( Df * Df - DDf_half * f);
                };
                
//                // Newton iteration step.
//                auto Step = [b,c,d]( const Real x )
//                {
//                    const Real f        = ( ( x + b) * x + c) * x + d;
//                    const Real Df       = ( c_3 * x + c_2 * b) * x + c;
//
//                    return -f/Df;
//                };
                
//                dump(D);
//                dump(y_infl);
                
                Real x  = (D < c_0 ) ? x_infl : x_infl - Sign(y_infl) * c_2/c_3 * Sqrt(D);
//                dump(x);
                
                Real delta_x = Step(x);
                x += delta_x;
                
                delta_x = Step(x);
                x += delta_x;
                
//                dump(x);
                
                int iter = 2;
                
                while( (Abs(delta_x) > tol) && (iter < max_iter) )
                {
                    ++iter;
                    
                    delta_x = Step(x);
                    x += delta_x;
                }
                
                x_0 = x;
            }
        }
        
        x_out[0] = x_0;
        
        const Real b_ = x_0 + b;
        const Real c_ = b_ * x_0 + c;
        
        const Real discriminant = b_ * b_ -  Scalar::Four<Real> * c_;
        
        if( discriminant >= c_0 )
        {
            const Real sqrt_discriminant = Sqrt(discriminant);
            
            x_out[1] = ( - b_ - sqrt_discriminant ) * Scalar::Half<Real>;
            x_out[2] = ( - b_ + sqrt_discriminant ) * Scalar::Half<Real>;
        }
        else
        {
            x_out[1] = infty;
            x_out[2] = infty;
        }
    }

//    template<typename Real>
//    void RealCubicSolve( const Real b, const Real c, const Real d, mptr<Real> x,
//        const Real tol = 64 * Scalar::eps<Real>, const int max_iter = 32
//    )
//    {
//        // Computes the real roots of the cubic equation x^3 + b * x^2 + c * x + d = 0.
//
//        static_assert( Scalar::RealQ<Real>, "Input parameters must be of real floating point types.");
//
//        constexpr Real c_0   = 0;
//        constexpr Real c_3   = 3;
//
//        constexpr Real half  = Inv<Real>(2);
//
//        constexpr Real C_0 = Frac<Real>(2,27);
//        constexpr Real C_1 = Frac<Real>(1,3);
//        constexpr Real C_2 = Frac<Real>(2,3);
//
//        static constexpr Real infty = std::numeric_limits<Real>::max();
//
//        // Convert to depressed form:
//        // t = x - shift
//        // g(t) := f(t+shift) = t^3 - p * t + q
//
//        const Real bb = b * b;
//
//        const Real p  = bb * C_1 - c;
//
//        const Real shift = - b * C_1;
//
//        const Real q  = (C_0 * bb - c * C_1) * b + d;
//
//        dump(p);
//        dump(q);
//
//        // Now we apply the iterative method from
//        // Deiters, Macias-Salinas - Calculation of Densities from Cubic Equations of State: Revisited
//
//        // cf. https://de.wikipedia.org/wiki/Kubische_Gleichung#Schnelle_numerische_Berechnung
//
//
//        if( p == c_0 )
//        {
//            // Three-fold root.
//
//            const Real t = - std::cbrt( q );
//
//            x[0] = t + shift;
//            x[1] = t + shift;
//            x[2] = t + shift;
//
//            return;
//        }
//
//        // General Halley iteration.
//        auto Step = [p,q]( const Real t )
//        {
//            const Real tt = t * t;
//
//            const Real g        = (tt - p) * t + q;
//            const Real Dg       = c_3 * tt - p;
//            const Real DDg_half = c_3 * t;
//
//            return ( Dg * g ) / ( Dg * Dg - DDg_half * g);
//        };
//
//        Real t_0 = 0;
//
//        if( q != c_0 )
//        {
//            // First Halley iteration.
//            Real t  = ( p < c_0 ) ? q/p  : ( (q > c_0 ) ? -C_2 * Sqrt(c_3 * p) : C_2 * Sqrt(c_3 * p) );
//            dump(t);
//
//
//            if( p < c_0 )
//            {
//                dump(q/p)
//            }
//            else if ( q > c_0 )
//            {
//                dump( -C_2 * Sqrt(c_3 * p) )
//            }
//            else
//            {
//                dump( C_2 * Sqrt(c_3 * p) )
//            }
//
//            Real delta_t = Step(t);
//
//            t -= delta_t;
//
//            dump(t);
//            int iter = 1;
//
//            while( (Abs(delta_t) > tol) && (iter < max_iter) )
//            {
//                ++iter;
//
//                delta_t = Step(t);
//                t -= delta_t;
//
//                dump(t);
//            }
//
////            if( iter >= max_iter )
////            {
////                const Real f = (t * t + p) * t + q;
////
////                wprint( std::string("RealCubicSolve( ") + ToString(b) + ", " + ToString(c) + ", " + ToString(d) + "," + ToString(tol) + ", " + ToString(max_iter) + " ) did not converge. Residual = " + ToString(f) + ", last step size = " + ToString(delta_t) + ".");
////            }
//
//            t_0 = t;
//        }
//
//        x[0] = t_0 + shift;
//
//        const Real discriminant = Scalar::Four<Real> * p - c_3 * t_0 * t_0;
//
//        if( discriminant >= c_0 )
//        {
//            const Real sqrt_discriminant = Sqrt(discriminant);
//
//            x[1] = shift - half * ( t_0 + sqrt_discriminant);
//            x[2] = shift - half * ( t_0 - sqrt_discriminant);
//        }
//        else
//        {
//            x[1] = infty;
//            x[2] = infty;
//        }
//    }
    
    
    
    template<typename Real>
    void RealCubicSolve( const Real a, const Real b, const Real c, const Real d, mptr<Real> x,
        const Real tol = 64 * Scalar::eps<Real>, const int max_iter = 64
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
            
            const Real a_inv = Inv<Real>(a);
            RealCubicSolve_Iterative( b * a_inv, c * a_inv, d * a_inv, x, tol, max_iter );
        }
        
    }
}

 
