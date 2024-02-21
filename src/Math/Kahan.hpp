#pragma once

namespace Tools
{
    template<typename Real>
    force_inline Real Det2D_Kahan( const Real a, const Real b, const Real c, const Real d )
    {
        // https://www.johndcook.com/blog/2020/05/31/kahan-determinant/
        
        // Computes a * d - b * c with less rounding error.
        
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
    force_inline Real Dot2D_Kahan(
        const Real u_0, const Real u_1, const Real v_0, const Real v_1
    )
    {
        return Det2D_Kahan( u_0, u_1, -v_1, v_0 );
    }
    
    template<typename Real>
    force_inline Real Dot2D_Kahan( cptr<Real> u, cptr<Real> v )
    {
        return Det2D_Kahan( u[0], u[1], -v[1], v[0] );
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
    
} // namespace Tools

 
