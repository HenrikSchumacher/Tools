#pragma once

namespace Tools
{
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S, typename Int = Size_T>
    [[nodiscard]] TOOLS_FORCE_INLINE Scalar::Real<S> norm_1(
        cptr<S> x, const Int n = static_cast<Int>(N), const Int thread_count = 1
    )
    {
        using T = Scalar::Real<S>;
        
        {
            #pragma float_control(precise, off)
            
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
    }
    
} // namespace Tools
