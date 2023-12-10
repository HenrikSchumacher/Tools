#pragma once

namespace Tools
{
    template<bool fmaQ = true, typename Real>
    force_inline Real SignedVolume3D_Naive(
        cptr<Real> x, cptr<Real> y, cptr<Real> z, cptr<Real> w
    )
    {
        using std::fma;
        
        const Real A [3][3] {
            { y[0] - x[0], z[0] - x[0], w[0] - x[0] },
            { y[1] - x[1], z[1] - x[1], w[1] - x[1] },
            { y[2] - x[2], z[2] - x[2], w[2] - x[2] }
        };
        
        if constexpr( fmaQ )
        {
            const Real r [3] {
                fma( A[1][1], A[2][2], - A[1][2] * A[2][1]),
                fma( A[1][2], A[2][0], - A[1][0] * A[2][2]),
                fma( A[1][0], A[2][1], - A[1][1] * A[2][0])
            };
            
            return fma( A[0][2], r[2], fma( A[0][1], r[1], A[0][0] * r[0] ) );
        }
        else
        {
            return
                A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
                +
                A[0][1] * (A[1][2] * A[2][0] - A[1][0] * A[2][2])
                +
                A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
        }
    }
    
    template<bool fmaQ = true, typename Real>
    force_inline int Oriented3D_Naive(
        cptr<Real> x, cptr<Real> y, cptr<Real> z, cptr<Real> w
    )
    {
        return Sign( SignedVolume3D_Naive<fmaQ>( x, y, z, w ) );
    }
    
    
    template<bool diffsafeQ = false, bool fmaQ = true, typename Real>
    force_inline Interval<Real> SignedVolume3D_IA(
        cptr<Real> x, cptr<Real> y, cptr<Real> z, cptr<Real> w
    )
    {
        using I_T = Interval<Real>;
        using S_T = Singleton<Real>;
        
        if constexpr ( diffsafeQ )
        {
            const S_T A [3][3] {
                { S_T(y[0] - x[0]), S_T(z[0] - x[0]), S_T(w[0] - x[0]) },
                { S_T(y[1] - x[1]), S_T(z[1] - x[1]), S_T(w[1] - x[1]) },
                { S_T(y[2] - x[2]), S_T(z[2] - x[2]), S_T(w[2] - x[2]) }
            };
            
            if constexpr( fmaQ )
            {
                const I_T r [3] {
                    fma( A[1][1], A[2][2], - A[1][2] * A[2][1]),
                    fma( A[1][2], A[2][0], - A[1][0] * A[2][2]),
                    fma( A[1][0], A[2][1], - A[1][1] * A[2][0])
                };

                return fma( A[0][2], r[2], fma( A[0][1], r[1], A[0][0] * r[0] ) );
            }
            else
            {
                return
                    A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
                    +
                    A[0][1] * (A[1][2] * A[2][0] - A[1][0] * A[2][2])
                    +
                    A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
            }
        }
        else
        {
            cptr<S_T> X = reinterpret_cast<cptr<S_T>>(x);
            cptr<S_T> Y = reinterpret_cast<cptr<S_T>>(y);
            cptr<S_T> Z = reinterpret_cast<cptr<S_T>>(z);
            cptr<S_T> W = reinterpret_cast<cptr<S_T>>(w);

            const I_T A [3][3] {
                { Y[0] - X[0], Z[0] - X[0], W[0] - X[0] },
                { Y[1] - X[1], Z[1] - X[1], W[1] - X[1] },
                { Y[2] - X[2], Z[2] - X[2], W[2] - X[2] }
            };
            
            if constexpr( fmaQ )
            {
                const I_T r [3] {
                    fma( A[1][1], A[2][2], - A[1][2] * A[2][1]),
                    fma( A[1][2], A[2][0], - A[1][0] * A[2][2]),
                    fma( A[1][0], A[2][1], - A[1][1] * A[2][0])
                };

                return fma( A[0][2], r[2], fma( A[0][1], r[1], A[0][0] * r[0] ) );
            }
            else
            {
                return
                    A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
                    +
                    A[0][1] * (A[1][2] * A[2][0] - A[1][0] * A[2][2])
                    +
                    A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
            }
        }
    }
    
    
    template<bool diffsafeQ = false, bool fmaQ = true, typename Real>
    force_inline int Oriented3D_IA(
        cptr<Real> x, cptr<Real> y, cptr<Real> z, cptr<Real> w
    )
    {
        return Sign( SignedVolume3D_IA<diffsafeQ,fmaQ>(x,y,z,w) );
    }
    
} // namespace Tools
