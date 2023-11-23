#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
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
        
        constexpr auto ox = COND( opx == Op::Conj,
                []( const x_T & z ){ return scalar_cast<T>( Conj<x_T>(z) ); },
                []( const x_T & z ){ return scalar_cast<T>( z ); }
        );
        
        constexpr auto oy = COND( opy == Op::Conj,
                []( const y_T & z ){ return scalar_cast<T>( Conj<y_T>(z) ); },
                []( const y_T & z ){ return scalar_cast<T>( z ); }
        );
        
        if constexpr ( N == VarSize )
        {
            if constexpr ( parQ == Parallel )
            {
                return ParallelDoReduce(
                    [=]( const Size_T i )
                    {
                        return ox(x[i]) * oy(y[i]);
                    },
                    []( const Size_T thread, const T & value, T & result )
                    {
                        result += value;
                    },
                    n, static_cast<T>(0), thread_count
                );
            }
            else
            {
                T sum = 0;
                
                for( Size_T i = 0; i < n; ++i )
                {
                    sum += ox(x[i]) * oy(y[i]);
                }
                
                return sum;
            }
        }
        else
        {
            T sum = 0;
            
            for( Size_T i = 0; i < N; ++i )
            {
                sum += ox(x[i]) * oy(y[i]);
            }
            return sum;
        }
    }
    
} // namespace Tools

