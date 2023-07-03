#pragma once

namespace Tools
{
    template <
        Tensors::Op opx = Tensors::Op::Id, Tensors::Op opy = Tensors::Op::Id,
        Parallel_T parQ = Sequential,
        typename R, typename S
    >
    force_inline
    decltype( R(1)*S(1) ) dot_buffers(
        ptr<R> x, ptr<S> y, const Size_T n, const Size_T thread_count = 1
    )
    {
        // Computes inner product <opx(x), opx(y)> of two vectors x and y.

        using Op = Tensors::Op;
        
        check_sequential<parQ>( "combine_scatter_write", thread_count );
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "dot_buffers: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "dot_buffers: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        constexpr auto ox = COND( opx == Op::Conj,
                []( const R & z ){ return Scalar::Conj<R>(z); },
                []( const R & z ){ return z; }
        );
        
        constexpr auto oy = COND( opy == Op::Conj,
                []( const S & z ){ return Scalar::Conj<S>(z); },
                []( const S & z ){ return z; }
        );
        
        using T = decltype( R(1)*S(1) );
        
        if constexpr ( parQ == Parallel )
        {
            return ParallelDoReduce(
                [=]( const Size_T i ) -> T
                {
                    return scalar_cast<T>( ox(x[i]) ) * scalar_cast<T>( oy(y[i]) );
                },
                []( const Size_T thread, const T & value, T & result ) -> T
                {
                    result += value;
                },
                n, static_cast<T>(0), thread_count
            );
        }
        else
        {
            decltype( R(1) * S(1) ) sum = 0;
            
            for( Size_T i = 0; i < n; ++i )
            {
                sum += scalar_cast<T>( ox(x[i]) ) * scalar_cast<T>( oy(y[i]) );
            }
            
            return sum;
        }
    }

    template <
        Tensors::Op opx = Tensors::Op::Id, Tensors::Op opy = Tensors::Op::Id, Size_T n,
        typename R, typename S
    >
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y )
    {
        // Computes inner product <opx(x), opx(y)> of two vectors x and y.
        
        using Op = Tensors::Op;
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "dot_buffers: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "dot_buffers: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        constexpr auto ox = COND( opx == Op::Conj,
                []( const R & z ){ return Scalar::Conj<R>(z); },
                []( const R & z ){ return z; }
        );
        
        constexpr auto oy = COND( opy == Op::Conj,
                []( const S & z ){ return Scalar::Conj<S>(z); },
                []( const S & z ){ return z; }
        );
        
        using T = decltype( R(1)*S(1) );
        
        T sum = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            sum += scalar_cast<T>( ox(x[i]) ) * scalar_cast<T>( oy(y[i]) );
        }
        return sum;
    }
    
} // namespace Tools

