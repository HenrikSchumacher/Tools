#pragma once

namespace Tools
{
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S, typename Int = Size_T >
    [[nodiscard]] force_inline Scalar::Real<S> total_buffer(
        cptr<S> x, const Int n = static_cast<Int>(N), const Int thread_count = 1
    )
    {
        using T = Scalar::Real<S>;
        
        {
            #pragma float_control(precise, off)
            
            return DoReduce<N,parQ>(
                [x]( const Int i )
                {
                    return x[i];
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
