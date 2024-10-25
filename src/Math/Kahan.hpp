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
    
    template<typename Real>
    force_inline bool ConvexPlanarQuadrilateralQ_Kahan(
        cptr<Real> x_0, cptr<Real> x_1, cptr<Real> x_2, cptr<Real> x_3
    )
    {
        // TODO: One can probably boil down the number of subtractions by half here.
        
        using T = signed char;
        
        const T signs [4] {
            Oriented2D_Kahan<T>( x_1, x_2, x_3 ),
            Oriented2D_Kahan<T>( x_2, x_3, x_0 ),
            Oriented2D_Kahan<T>( x_3, x_0, x_1 ),
            Oriented2D_Kahan<T>( x_0, x_1, x_2 )
        };
        
        return (
            ( signs[0] != T(0)     )
            &&
            ( signs[0] == signs[1] )
            &&
            ( signs[1] == signs[2] )
            &&
            ( signs[2] == signs[3] )
        );
    }
    
    template<typename Real>
    force_inline bool LineSegmentsIntersectQ_Kahan(
        cptr<Real> x_0, cptr<Real> x_1, cptr<Real> y_0, cptr<Real> y_1
    )
    {
        // x_0, x_1, y_0, y_1 are supposed to be points in the plane.
        
        // TODO: One can probably boil down the number of subtractions by half here.
        
        using Sint = signed char;
        
        const Sint signs [4] {
            Oriented2D_Kahan<Sint>( x_1, y_0, y_1 ),
            Oriented2D_Kahan<Sint>( y_0, y_1, x_0 ),
            Oriented2D_Kahan<Sint>( y_1, x_0, x_1 ),
            Oriented2D_Kahan<Sint>( x_0, x_1, y_0 )
        };
        
        if( (signs[0] == 0) || (signs[1] == 0) || (signs[2] == 0) || (signs[3] == 0) )
        {
            eprint(std::string("LineSegmentsIntersectQ_Kahan detected degenerate configuration. \n")
            + "\tx_0 = " + ArrayToString(x_0,{2}) + "\n"
            + "\tx_1 = " + ArrayToString(x_1,{2}) + "\n"
            + "\ty_0 = " + ArrayToString(y_0,{2}) + "\n"
            + "\ty_1 = " + ArrayToString(y_1,{2}) );
        }
        
        return (
            ( signs[0] != Sint(0)  )
            &&
            ( signs[1] != Sint(0)  )
            &&
            ( signs[0] != signs[1] )
            &&
            ( signs[0] == signs[3] )
            &&
            ( signs[1] == signs[2] )
        );
    }
    
} // namespace Tools

 
