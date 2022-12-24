#pragma once

namespace Tools
{
    
    template <typename R, typename S>
    force_inline
    decltype( R(1) * S(1) ) dot_buffers(
        const R * restrict const x,
        const S * restrict const y,
        const size_t n
    )
    {
        decltype( R(1) * S(1) ) sum = 0;
        for( size_t i = 0; i < n; ++i )
        {
            sum += x[i] * y[i];
        }
        return sum;
    }

    template <size_t n, typename R, typename S>
    force_inline
    decltype( R(1) * S(1) ) scale_buffer(
        const R * restrict const x,
        const S * restrict const y
    )
    {
        decltype( R(1) * S(1) ) sum = 0;
        for( size_t i = 0; i < n; ++i )
        {
            sum += x[i] * y[i];
        }
        return sum;
    }

    
} // namespace Tools

