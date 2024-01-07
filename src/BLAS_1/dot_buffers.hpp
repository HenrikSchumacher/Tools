#pragma once

namespace Tools
{
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, 
        Op opx = Op::Id, Op opy = Op::Id,
        typename x_T, typename y_T
    >
    [[nodiscard]] force_inline
    decltype( x_T(1) * y_T(1) ) dot_buffers(
        cptr<x_T> x, cptr<y_T> y, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        // Computes inner product <opx(x), opx(y)> of two vectors x and y.

        check_sequential<parQ>( "dot_buffers", thread_count );
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "dot_buffers: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "dot_buffers: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        using T = decltype( x_T(1) * y_T(1) );
        
        return DoReduce<N,parQ>(
            [x,y]( const Size_T i ) -> T
            {
                return
                    scalar_cast<T>( opx == Op::Conj ? Conj(x[i]) : x[i] )
                    *
                    scalar_cast<T>( opy == Op::Conj ? Conj(y[i]) : y[i] );
            },
            []( const T & value, T & result )
            {
                result += value;
            },
            T(0), n, thread_count
        );
    }
    
} // namespace Tools

