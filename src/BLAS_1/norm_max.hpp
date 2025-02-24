#pragma once

namespace Tools
{
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE Scalar::Real<S> norm_max(
        cptr<S> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        if constexpr ( Scalar::RealQ<S> )
        {
            return MaxBy<N,parQ>( [x]( const Size_T i ){ return Abs(x[i]); }, n, thread_count );
        }
        else
        {
            return Sqrt(
                MaxBy<N,parQ>( [x]( const Size_T i ){ return AbsSquared(x[i]); }, n, thread_count )
            );
        }
    }
    
} // namespace Tools


