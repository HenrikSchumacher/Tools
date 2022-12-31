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
    
    template <typename R, typename S>
    force_inline
    decltype( R(1) * S(1) ) dot_buffers(
        const R * restrict const x,
        const S * restrict const y,
        const size_t n,
        const size_t thread_count
    )
    {
        if( thread_count <= 1 )
        {
            return dot_buffers(x,y,n);
        }
        else
        {
            decltype( R(1) * S(1) ) sum = 0;
            
            #pragma omp parallel for num_threads( thread_count ) reduction( + : sum)
            for( size_t thread = 0; thread < thread_count; ++thread )
            {
                const size_t i_begin = JobPointer(n,thread_count,thread  );
                const size_t i_end   = JobPointer(n,thread_count,thread+1);
                
                decltype( R(1) * S(1) ) local_sum = 0;
                for( size_t i = i_begin; i < i_end; ++i )
                {
                    local_sum += x[i] * y[i];
                }
                
                sum += local_sum;
            }
            return sum;
        }
    }

    template <size_t n, typename R, typename S>
    force_inline
    decltype( R(1) * S(1) ) dot_buffers(
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

