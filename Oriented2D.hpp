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
        const Real ad = A[0] * A[3];
        const Real bc = A[1] * A[2];
        
        return (ad > bc) - (ad < bc);
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
        const Real A = (y[0] - x[0]) * (z[1] - x[1]);
        const Real B = (y[1] - x[1]) * (z[0] - x[0]);

        return (A > B) - (A < B);
    }
    
    

    template<typename Real>
    force_inline Real Det2D_Kahan( cptr<Real> A )
    {
        // https://www.johndcook.com/blog/2020/05/31/kahan-determinant/

//        const Real w = A[1] * A[2];
//        const Real e = std::fma(-A[1],A[2], w);
//        const Real f = std::fma( A[0],A[3],-w);
//    
//        return e+f;
        
        const Real w = - A[1] * A[2];
        const Real e = std::fma( A[1], A[2], w );
        const Real f = std::fma( A[0], A[3], w );
        
        return f-e;
    }
    
    
    template<typename Real>
    force_inline int DetSign2D_Kahan( cptr<Real> A )
    {
        const Real w = - A[1] * A[2];
        const Real e = std::fma( A[1], A[2], w );
        const Real f = std::fma( A[0], A[3], w );
        
        return (f > e) - (f < e);
    }
    
    
    template<typename Real>
    force_inline Real SignedVolume2D_Kahan( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        const Real A [2][2] {
            { y[0] - x[0], z[1] - x[1] },
            { y[1] - x[1], z[0] - x[0] }
        };
        
        return Det2D_Kahan( &A[0][0] );
    }
    

    template<typename Real>
    force_inline int Oriented2D_Kahan( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        // TODO: If this is the best solution, then I have inline here.
    
        const Real A [2][2] {
            { y[0] - x[0], z[1] - x[1] },
            { y[1] - x[1], z[0] - x[0] }
        };
        
        const Real w = - A[0][1] * A[1][0];
        const Real e = std::fma( A[0][1], A[1][0], w );
        const Real f = std::fma( A[0][0], A[1][1], w );
        
        return (f > e) - (f < e);
    }
    
    
    
    template<bool diffsafeQ = false, bool fmaQ = true, typename Real>
    force_inline Interval<Real> SignedVolume2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
//        using I_T = Interval<Real>;
        using S_T = Singleton<Real>;
        
        if constexpr ( diffsafeQ )
        {
            if constexpr ( fmaQ )
            {
                return fma( S_T(y[0] - x[0]), S_T(z[1] - x[1]), S_T(x[1] - y[1]) * S_T(z[0] - x[0]) );
            }
            else
            {
                return S_T(y[0] - x[0]) * S_T(z[1] - x[1]) + S_T(x[1] - y[1]) * S_T(z[0] - x[0]);
            }
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            if constexpr ( fmaQ )
            {
                return fma( (Y[0] - X[0]), (Z[1] - X[1]), (X[1] - Y[1]) * (Z[0] - X[0]) );
            }
            else
            {
                return (Y[0] - X[0]) * (Z[1] - X[1]) + (X[1] - Y[1]) * (Z[0] - X[0]);
            }
        }
    }
    
    template<typename Real>
    force_inline int DetSign2D_IA( cptr<Real> A_ )
    {
        using I_T = Interval<Real>;
        using S_T = Singleton<Real>;
        
        cptr<S_T> A = reinterpret_cast<cptr<S_T>>(A_);
        
        const I_T ad = A[0] * A[3];
        const I_T bc = A[1] * A[2];

        return (ad > bc) - (ad < bc);
    }
    
    template<bool diffsafeQ, typename Real>
    force_inline int Oriented2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        using I_T = Interval<Real>;
        using S_T = Singleton<Real>;
        
        if constexpr ( diffsafeQ )
        {
            const I_T A = S_T(y[0] - x[0]) * S_T(z[1] - x[1]);
            const I_T B = S_T(y[1] - x[1]) * S_T(z[0] - x[0]);

            return (A > B) - (A < B);
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            const I_T A { (Y[0] - X[0]) * (Z[1] - X[1]) };
            const I_T B { (Y[1] - X[1]) * (Z[0] - X[0]) };
            
            return (A > B) - (A < B);
        }
    }
    
} // namespace Tools
