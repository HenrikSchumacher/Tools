#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y, const Size_T n )
    {
        using T = decltype( R(1)*S(1) );
        T sum = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            sum += scalar_cast<T>(x[i]) * scalar_cast<T>(y[i]);
        }
        return sum;
    }
    
    template <typename R, typename S>
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y, const Size_T n, const Size_T thread_count )
    {
        using T = decltype( R(1)*S(1) );
        
        if( thread_count <= 1 )
        {
            return dot_buffers(x,y,n);
        }
        else
        {
            decltype( R(1) * S(1) ) sum = 0;
            
            return ParallelDoReduce(
                [=]( const Size_T i ) -> T
                {
                    return scalar_cast<T>(x[i]) * scalar_cast<T>(y[i]);
                },
                []( const Size_T thread, const T & value, T & result ) -> T
                {
                    result += value;
                },
                n,
                static_cast<T>(0),
                thread_count
            );
        }
    }

    template <Size_T n, typename R, typename S>
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y )
    {
        using T = decltype( R(1)*S(1) );
        T sum = 0;
        for( Size_T i = 0; i < n; ++i )
        {
            sum += scalar_cast<T>(x[i]) * scalar_cast<T>(y[i]);
        }
        return sum;
    }
    
} // namespace Tools

