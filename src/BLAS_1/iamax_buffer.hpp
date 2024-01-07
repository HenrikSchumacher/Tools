#pragma once

namespace Tools
{
    template <Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] force_inline Size_T
    iamax_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1 )
    {
        // Find the first position i such that z[i] has maximal modulus.
        
        if constexpr ( Scalar::RealQ<S> )
        {
            return MaximumBy<N,parQ>(
                [z]( const Size_T i ){ return Abs(z[i]); }, n, thread_count
            ).first;
        }
        else
        {
            return MaximumBy<N,parQ>(
                [z]( const Size_T i ){ return AbsSquared(z[i]); }, n, thread_count
            ).first;
        }
    }
    
    template <Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] force_inline Size_T
    iamin_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1 )
    {
        // Find the first position i such that z[i] has minimal modulus.
        
        if constexpr ( Scalar::RealQ<S> )
        {
            return MinimumBy<N,parQ>(
                [z]( const Size_T i ){ return Abs(z[i]); }, n, thread_count
            ).first;
        }
        else
        {
            return MinimumBy<N,parQ>(
                [z]( const Size_T i ){ return AbsSquared(z[i]); }, n, thread_count
            ).first;
        }
    }
    
} // namespace Tools
