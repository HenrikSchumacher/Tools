#pragma once

namespace Tools
{
    
    template<bool fmaQ = true, typename Real>
    force_inline Real Det2D_Naive( cptr<Real> A )
    {
        if constexpr ( fmaQ )
        {
            return std::fma( A[0], A[3], - A[1] * A[2] );
        }
        else
        {
            return A[0] * A[3] - A[1] * A[2];
        }
    }
    
    template<typename Real>
    force_inline int DetSign2D_Naive( cptr<Real> A )
    {
        return DifferenceSign( A[0] * A[3], A[1] * A[2] );
    }
    
    template<bool fmaQ = true, typename Real>
    force_inline Real SignedVolume2D_Naive( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        const Real A [2][2] {
            { y[0] - x[0], z[1] - x[1] },
            { y[1] - x[1], z[0] - x[0] }
        };
    
        return Det2D_Naive<fmaQ>( &A[0][0] );
    }
    
    template<typename Real>
    force_inline int Oriented2D_Naive( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return DifferenceSign( (y[0] - x[0]) * (z[1] - x[1]), (y[1] - x[1]) * (z[0] - x[0]) );
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
    
    template<typename Real>
    force_inline int DetSign2D_Kahan( const Real a, const Real b, const Real c, const Real d )
    {
        const Real w = - b * c;
        const Real e = std::fma( b, c, w );
        const Real f = std::fma( a, d, w );
        
        return DifferenceSign(f,e);
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
    
    
    template<typename Real>
    force_inline int DetSign2D_Kahan( cptr<Real> A )
    {
        return DetSign2D_Kahan( A[0], A[1], A[2], A[3] );
    }
    
    
    template<typename Real>
    force_inline Real SignedVolume2D_Kahan( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return Det2D_Kahan( y[0] - x[0], z[1] - x[1], y[1] - x[1], z[0] - x[0] );
    }
    

    template<typename Real>
    force_inline int Oriented2D_Kahan( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        return DetSign2D_Kahan( y[0] - x[0], z[1] - x[1], y[1] - x[1], z[0] - x[0] );
    }
    
    

    
    template<bool fmaQ = true, typename Real>
    force_inline Interval<Real> Det2D_IA(
        const Singleton<Real> a, const Singleton<Real> b,
        const Singleton<Real> c, const Singleton<Real> d
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
    
    template<bool fmaQ = true, typename Real>
    force_inline Interval<Real> Det2D_IA(
        const Real a, const Real b,
        const Real c, const Real d
    )
    {
        using S_T = Singleton<Real>;
        
        return Det2D_IA<fmaQ>( S_T(a), S_T(b), S_T(c), S_T(d) );
    }
    
    template<bool fmaQ = true, typename Real>
    force_inline Interval<Real> Det2D_IA(
        const Interval<Real> a, const Interval<Real> b,
        const Interval<Real> c, const Interval<Real> d
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
    
    template<bool diffsafeQ = false, bool fmaQ = true, typename Real>
    force_inline Interval<Real> SignedVolume2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        using S_T = Singleton<Real>;
        
        if constexpr ( diffsafeQ )
        {
            return Det2D_IA<fmaQ>( y[0] - x[0], y[1] - x[1], z[0] - x[0], z[1] - x[1] );
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            return Det2D_IA<fmaQ>( Y[0] - X[0], Y[1] - X[1], Z[0] - X[0], Z[1] - X[1] );
        }
    }
    
    

    
    template<typename Real>
    force_inline int DetSign2D_IA(
        const Singleton<Real> a, const Singleton<Real> b,
        const Singleton<Real> c, const Singleton<Real> d
    )
    {
        return DifferenceSign( a * d, b * c );
    }
    
    template<typename Real>
    force_inline int DetSign2D_IA(
        const Real a, const Real b,
        const Real c, const Real d
    )
    {
        using S_T = Singleton<Real>;
        
        return DifferenceSign( S_T(a) * S_T(d), S_T(b) * S_T(c) );
    }
    
    template<typename Real>
    force_inline Interval<Real> DetSign2D_IA(
        const Interval<Real> a, const Interval<Real> b,
        const Interval<Real> c, const Interval<Real> d
    )
    {
        return DifferenceSign( a * d, b * c );
    }
    
    template<typename T>
    force_inline int DetSign2D_IA( cptr<T> A_ )
    {
        return DetSign2D_IA( A[0], A[1], A[2], A[3] );
    }
    
    template<bool diffsafeQ, typename Real>
    force_inline int Oriented2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        using S_T = Singleton<Real>;
        
        if constexpr ( diffsafeQ )
        {
            // This calls the Real overload of DetSign2D_IA.
            return DetSign2D_IA( y[0] - x[0], y[1] - x[1], z[0] - x[0], z[1] - x[1] );
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            // This calls the Interval<Real> overload of DetSign2D_IA.
            return DetSign2D_IA( Y[0] - X[0], Y[1] - X[1], Z[0] - X[0], Z[1] - X[1] );
        }
    }
    
} // namespace Tools
