#pragma once

namespace Tools
{
    template<bool fmaQ = true, typename Real>
    force_inline Real SignedVolume2D_Naive( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        if constexpr ( fmaQ )
        {
            return std::fma( (y[0] - x[0]), (z[1] - x[1]), (x[1] - y[1]) * (z[0] - x[0]) );
        }
        else
        {
            return (y[0] - x[0]) * (z[1] - x[1]) + (x[1] - y[1]) * (z[0] - x[0]);
        }
    }
    
    template<typename Real>
    force_inline Real Oriented2D_Naive( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        const Real A = (y[0] - x[0]) * (z[1] - x[1]);
        const Real B = (y[1] - x[1]) * (z[0] - x[0]);

        return static_cast<Real>( (A > B) - (A < B) );
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
    
    
    template<bool diffsafeQ, typename Real>
    force_inline Real Oriented2D_IA( cptr<Real> x, cptr<Real> y, cptr<Real> z )
    {
        using I_T = Interval<Real>;
        using S_T = Singleton<Real>;
        
        if constexpr ( diffsafeQ )
        {
            // Beware, it is fully intentional that we apply I only to one factor!
            const I_T A = S_T(y[0] - x[0]) * S_T(z[1] - x[1]);
            const I_T B = S_T(y[1] - x[1]) * S_T(z[0] - x[0]);

            return static_cast<Real>( (A > B) - (A < B) );
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            
            const I_T A { (Y[0] - X[0]) * (Z[1] - X[1]) };
            const I_T B { (Y[1] - X[1]) * (Z[0] - X[0]) };
            
            return static_cast<Real>( (A > B) - (A < B) );
        }
    }
    
} // namespace Tools
