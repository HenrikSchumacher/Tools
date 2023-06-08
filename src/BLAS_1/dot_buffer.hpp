#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y, const std::size_t n )
    {
        using T = decltype( R(1)*S(1) );
        T sum = 0;
        for( std::size_t i = 0; i < n; ++i )
        {
            sum += scalar_cast<T>(x[i]) * scalar_cast<T>(y[i]);
        }
        return sum;
    }
    
    template <typename R, typename S>
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y, const std::size_t n, const std::size_t thread_count )
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
                [=]( const std::size_t i ) -> T
                {
                    return scalar_cast<T>(x[i]) * scalar_cast<T>(y[i]);
                },
                []( const std::size_t thread, const T & value, T & result ) -> T
                {
                    result += value;
                },
                n,
                static_cast<T>(0),
                thread_count
            );
        }
    }

    template <std::size_t n, typename R, typename S>
    force_inline
    decltype( R(1)*S(1) ) dot_buffers( ptr<R> x, ptr<S> y )
    {
        using T = decltype( R(1)*S(1) );
        T sum = 0;
        for( std::size_t i = 0; i < n; ++i )
        {
            sum += scalar_cast<T>(x[i]) * scalar_cast<T>(y[i]);
        }
        return sum;
    }
    
} // namespace Tools

