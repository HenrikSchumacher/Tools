#pragma once

namespace Tools
{
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] force_inline Scalar::Real<S> norm_1(
        cptr<S> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        using T = Scalar::Real<S>;
        
        return DoReduce<N,parQ>(
            [x]( const Size_T i )
            {
                return Abs(x[i]);
            },
            []( const T & value, T & result )
            {
                result += value;
            },
            Scalar::Zero<T>, n, thread_count
        );
    }
    
} // namespace Tools
