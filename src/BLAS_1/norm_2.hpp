#pragma once

namespace Tools
{

    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE Scalar::Real<S> norm_2_squared(
        cptr<S> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        using T = Scalar::Real<S>;
        
        {
            TOOLS_MAKE_FP_FAST()
            
            return DoReduce<N,parQ>(
                [x]( const Size_T i )
                {
                    return AbsSquared(x[i]);
                },
                []( const T & value, T & result )
                {
                    result += value;
                },
                Scalar::Zero<T>, n, thread_count
            );
        }
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE Scalar::Real<S> norm_2(
        cptr<S> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        return Sqrt( norm_2_squared<N,parQ>( x, n, thread_count ) );
    }
    
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename x_T >
    TOOLS_FORCE_INLINE void normalize(
        mptr<x_T> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        const Scalar::Real<x_T> a = Inv(norm_2<N,parQ>(x,n,thread_count));
        
        scale_buffer<N,parQ>(a,x,n,thread_count);
    }
    
    
    
} // namespace Tools
