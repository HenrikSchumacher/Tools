#pragma once

namespace Tools
{

    template<bool fmaQ = true, typename Real>
    force_inline Real Det2D_Naive( const Real a, const Real b, const Real c, const Real d )
    {
        if constexpr ( fmaQ )
        {
            return std::fma( a, d, - b * c );
        }
        else
        {
            return a * d - b * c;
        }
    }
    
    template<bool fmaQ = true, typename Real>
    force_inline Real Det2D_Naive( cptr<Real> A )
    {
        return Det2D_Naive<fmaQ>( A[0], A[1], A[2], A[3] );
    }

    template<typename Out_T = int, typename Real>
    force_inline Out_T DetSign2D_Naive( const Real a, const Real b, const Real c, const Real d )
    {
        return DifferenceSign<Out_T>( a * d, b * c );
    }
    
    template<typename Out_T = int, typename Real>
    force_inline Out_T DetSign2D_Naive( cptr<Real> A )
    {
        return DetSign2D_Naive<Out_T>( A[0], A[1], A[2], A[3] );
    }
    
    template<bool fmaQ = true, typename Real>
    force_inline Real SignedVolume2D_Naive( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return Det2D_Naive<fmaQ>( y[0] - x[0], z[0] - x[0], y[1] - x[1], z[1] - x[1] );
    }
    
    template<typename Out_T = int, typename Real>
    force_inline Out_T Oriented2D_Naive( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return DifferenceSign<Out_T>( (y[0] - x[0]) * (z[1] - x[1]), (y[1] - x[1]) * (z[0] - x[0]) );
    }
    
    

    template<typename Real>
    force_inline Real Det2D_Kahan( const Real a, const Real b, const Real c, const Real d )
    {
        // https://www.johndcook.com/blog/2020/05/31/kahan-determinant/

//        const Real w = b * c;
//        const Real e = std::fma(-b,c, w);
//        const Real f = std::fma( a,d,-w);
//
//        return e+f;
        
        const Real w = - b * c;
        const Real e = std::fma( b, c, w );
        const Real f = std::fma( a, d, w );
        
        return f - e;
    }
    
    template<typename Out_T = int, typename Real>
    force_inline Out_T DetSign2D_Kahan( const Real a, const Real b, const Real c, const Real d )
    {
        const Real w = - b * c;
        const Real e = std::fma( b, c, w );
        const Real f = std::fma( a, d, w );
        
        return DifferenceSign<Out_T>(f,e);
    }
    
    template<typename Real>
    force_inline Real Det2D_Kahan( cptr<Real> A )
    {
        return Det2D_Kahan( A[0], A[1], A[2], A[3] );
    }
    
    template<typename Real>
    force_inline Real Dot2D_Kahan( const Real a, const Real b, const Real c, const Real d )
    {
        return Det2D_Kahan( a, -b, c, d );
    }
    
    template<typename Real>
    force_inline Real Dot2D_Kahan( cptr<Real> u, cptr<Real> v )
    {
        return Det2D_Kahan( u[0], -u[1], v[0], v[1] );
    }
    
    
    template<typename Out_T = int, typename Real>
    force_inline Out_T DetSign2D_Kahan( cptr<Real> A )
    {
        return DetSign2D_Kahan<Out_T>( A[0], A[1], A[2], A[3] );
    }
    
    
    template<typename Real>
    force_inline Real SignedVolume2D_Kahan( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return Det2D_Kahan( y[0] - x[0], z[0] - x[0], y[1] - x[1], z[1] - x[1] );
    }
    

    template<typename Out_T = int, typename Real>
    force_inline Out_T Oriented2D_Kahan( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return DetSign2D_Kahan<Out_T>( y[0] - x[0], z[0] - x[0], y[1] - x[1], z[1] - x[1] );
    }
    
    

    
    template<bool fmaQ = true, typename Real, RoundingPolicy RP>
    force_inline Interval<Real,RP> Det2D_IA(
        const Singleton<Real,RP> a, const Singleton<Real,RP> b,
        const Singleton<Real,RP> c, const Singleton<Real,RP> d
    )
    {
        if constexpr ( fmaQ )
        {
            return fma( a, d, - b * c );
        }
        else
        {
            return a * d - b * c;
        }
    }
    
    
    
    template<bool fmaQ = true, typename Real, RoundingPolicy RP>
    force_inline Interval<Real,RP> Det2D_IA( cptr<Singleton<Real,RP>> A )
    {
        return Det2D_IA<fmaQ>( A[0], A[1], A[2], A[3] );
    }
    
    template<bool fmaQ = true, typename Real, RoundingPolicy RP>
    force_inline Interval<Real,RP> Det2D_IA(
        const Interval<Real,RP> a, const Interval<Real,RP> b,
        const Interval<Real,RP> c, const Interval<Real,RP> d
    )
    {
        if constexpr ( fmaQ )
        {
            return fma( a, d, - b * c );
        }
        else
        {
            return a * d - b * c;
        }
    }
    
    template<bool fmaQ = true, typename Real, RoundingPolicy RP>
    force_inline Interval<Real,RP> Det2D_IA( cptr<Interval<Real,RP>> A )
    {
        return Det2D_IA<fmaQ>( A[0], A[1], A[2], A[3] );
    }

    
    template<RoundingPolicy RP, bool diffsafeQ = false, bool fmaQ = true, typename Real>
    force_inline Interval<Real,RP> SignedVolume2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        using S_T = Singleton<Real,RP>;
        
        if constexpr ( diffsafeQ )
        {
            return Det2D_IA<fmaQ>( S_T(y[0] - x[0]), S_T(z[0] - x[0]), S_T(y[1] - x[1]), S_T(z[1] - x[1]) );
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            return Det2D_IA<fmaQ>( Y[0] - X[0], Z[0] - X[0], Y[1] - X[1], Z[1] - X[1] );
        }
    }
    
    

    
    template<typename Out_T = int, typename Real, RoundingPolicy RP>
    force_inline Out_T DetSign2D_IA(
        const Singleton<Real,RP> a, const Singleton<Real,RP> b,
        const Singleton<Real,RP> c, const Singleton<Real,RP> d
    )
    {
        return DifferenceSign<Out_T>( a * d, b * c );
    }
    
    template<typename Out_T = int, typename Real, RoundingPolicy RP>
    force_inline Out_T DetSign2D_IA( cptr<Singleton<Real,RP>> A )
    {
        return DetSign2D_IA( A[0], A[1], A[2], A[3] );
    }
    
//    template<typename Out_T = int, typename Real>
//    force_inline Out_T DetSign2D_IA(
//        const Real a, const Real b,
//        const Real c, const Real d
//    )
//    {
//        using S_T = Singleton<Real,RP>;
//        
//        return DifferenceSign<Out_T>( S_T(a) * S_T(d), S_T(b) * S_T(c) );
//    }

//    template<typename Out_T = int, typename Real>
//    force_inline Out_T DetSign2D_IA( cptr<Real> A )
//    {
//        return DetSign2D_IA<Out_T>( A[0], A[1], A[2], A[3] );
//    }
    
    template<typename Out_T = int, typename Real, RoundingPolicy RP>
    force_inline Out_T DetSign2D_IA(
        const Interval<Real,RP> a, const Interval<Real,RP> b,
        const Interval<Real,RP> c, const Interval<Real,RP> d
    )
    {
        return DifferenceSign<Out_T>( a * d, b * c );
    }
    
//    template<typename Out_T = int, typename Real>
//    force_inline Out_T DetSign2D_IA( cptr<Interval<Real>> A )
//    {
//        return DetSign2D_IA( A[0], A[1], A[2], A[3] );
//    }
    
    template<RoundingPolicy RP, typename Out_T, bool diffsafeQ, typename Real>
    force_inline Out_T Oriented2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        using S_T = Singleton<Real,RP>;
        
        if constexpr ( diffsafeQ )
        {
            // This calls the Singleton<Real,RP> overload of DetSign2D_IA.
            return DetSign2D_IA<Out_T>( S_T(y[0] - x[0]), S_T(z[0] - x[0]), S_T(y[1] - x[1]), S_T(z[1] - x[1]) );
        }
        else
        {
            
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            // This calls the Interval<Real,RP> overload of DetSign2D_IA.
            return DetSign2D_IA<Out_T>( Y[0] - X[0], Z[0] - X[0], Y[1] - X[1], Z[1] - X[1] );
        }
    }
    
    
    
    
    
    
    
    
    
    

    
    template<typename Out_T = int, typename Real>
    force_inline Out_T DetSign2D_Corrected( const Real a, const Real b, const Real c, const Real d )
    {
        ASSERT_FLOAT(Real);
        
        const auto [ad,ade] = TwoMulFMA(a,d);
        const auto [bc,bce] = TwoMulFMA(b,c);

        const Real diff = ad  - bc;
        const Real e    = bce - ade;
            
        return DifferenceSign<Out_T>(diff,e);
    }
    
    template<typename Out_T = int, typename Real>
    force_inline Out_T DetSign2D_Corrected( cptr<Real> A )
    {
        return DetSign2D_Corrected<Out_T>( A[0], A[1], A[2], A[3] );
    }

    template<typename Real>
    force_inline Real Det2D_Corrected( const Real a, const Real b, const Real c, const Real d )
    {
        ASSERT_FLOAT(Real);
        
        const auto [ad,ade] = TwoMulFMA(a,d);
        const auto [bc,bce] = TwoMulFMA(b,c);

        const Real diff = ad  - bc;
        const Real e    = bce - ade;
            
        return diff - e;
    }
    
    template<typename Real>
    force_inline Real Det2D_Corrected( cptr<Real> A )
    {
        return Det2D_Corrected( A[0], A[1], A[2], A[3] );
    }
    
} // namespace Tools
