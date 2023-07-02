#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename R, typename S
    >
    force_inline void add_to_buffer(
        ptr<R> x, mut<S> y,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "add_to_buffer", thread_count );
        
        Do<N,parQ>(
            [=]( const Size_T i )
            {
                y[i] += scalar_cast<S>(x[i]);
            },
            n, thread_count
        );
    }

} // namespace Tools
