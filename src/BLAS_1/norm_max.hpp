#pragma once

namespace Tools
{
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S, typename Int = Size_T >
    [[nodiscard]] TOOLS_FORCE_INLINE Scalar::Real<S> norm_max(
        cptr<S> x, const Int n = int_cast<Int>(N), const Int thread_count = Int(1)
    )
    {
        if constexpr ( Scalar::RealQ<S> )
        {
            return MaxBy<N,parQ>( [x]( const Int i ){ return Abs(x[i]); }, n, thread_count );
        }
        else
        {
            return Sqrt(
                MaxBy<N,parQ>( [x]( const Int i ){ return AbsSquared(x[i]); }, n, thread_count )
            );
        }
    }
    
} // namespace Tools


