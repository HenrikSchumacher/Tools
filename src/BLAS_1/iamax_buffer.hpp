#pragma once

namespace Tools
{
    
    template <
        Size_T N = VarSize,
        typename S
    >
    force_inline Size_T iamax_buffer( cptr<S> z, const Size_T n = N )
    {
        // Find the element i with maximal modulus.
        
        using R = typename Scalar::Real<S>;

        R max = 0;
        Size_T pos = 0;
        
        if constexpr ( N == VarSize )
        {
            for( Size_T i = 0; i < n; ++i )
            {
                R abs_z = Scalar::RealQ<S> ? Abs(z[i]) : AbsSquared(z[i]);
                if( abs_z > max )
                {
                    pos = i;
                    max = abs_z;
                }
            }
        }
        else
        {
            for( Size_T i = 0; i < N; ++i )
            {
                R abs_z = Scalar::RealQ<S> ? Abs(z[i]) : AbsSquared(z[i]);
                if( abs_z > max )
                {
                    pos = i;
                    max = abs_z;
                }
            }
        }
        
        return pos;
    }
    
    
    
    
    template <
        Size_T N = VarSize,
        typename R
    >
    force_inline Size_T imax_buffer( cptr<R> x, const Size_T n = N )
    {
        // Find the maximal element i.
    
        
        if constexpr ( N == VarSize )
        {
            R max = std::numeric_limits<R>::lowest();
            Size_T pos = 0;
            for( Size_T i = 0; i < n; ++i )
            {
                if( x[i] > max )
                {
                    pos = i;
                    max = x[i];
                }
            }
            return pos;
        }
        else
        {
            R max = std::numeric_limits<R>::lowest();
            Size_T pos = 0;
            for( Size_T i = 0; i < N; ++i )
            {
                if( x[i] > max )
                {
                    pos = i;
                    max = x[i];
                }
            }
            return pos;
        }
    }


    template <
        Size_T N = VarSize,
        typename R
    >
    force_inline Size_T imin_buffer( cptr<R> x, const Size_T n = N )
    {
        // Find the maximal element i.
    
        
        if constexpr ( N == VarSize )
        {
            R min = std::numeric_limits<R>::max();
            Size_T pos = 0;
            for( Size_T i = 0; i < n; ++i )
            {
                if( x[i] < min )
                {
                    pos = i;
                    min = x[i];
                }
            }
            return pos;
        }
        else
        {
            R min = std::numeric_limits<R>::max();
            Size_T pos = 0;
            for( Size_T i = 0; i < N; ++i )
            {
                if( x[i] < min )
                {
                    pos = i;
                    min = x[i];
                }
            }
            return pos;
        }
    }
    
} // namespace Tools

