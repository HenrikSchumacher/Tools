#pragma once

#include <numeric>

namespace Tools
{
    
    template<typename T, typename I, typename F>
    inline void parallel_prefix_scan(
        cptr<T>  a,
        mptr<T>  b,
        const I n,
        const F & f,
        const I thread_count
    )
    {
        // Assumint that f is commutative and (nearly) associative, we compute
        //
        //    b[0] = a[0];
        //    b[1] = f(b[1], a[1]);
        //    b[2] = f(b[1], a[2]);
        //    b[3] = f(b[2], a[3]);
        //    b[3] = f(b[3], a[4]);
        //    ...
        
        std::vector<T> S_buffer ( thread_count+1 );
        
        mptr<T> S = &S_buffer[0];
        
        S[0] = static_cast<T>(0);
        
        const I step = n / thread_count;
        const I corr = n % thread_count;
        
        // We don't know the neutral element of f, so we start with a[0];
        b[0] = S[0] = a[0];
        // b[0] is already known. We have to work on the remaining array.
        
        ParallelDo(
            [=]( const I thread )
            {
                // now each thread does the scan on its chunk
                const I i_begin = step*(thread  ) + (corr*(thread  ))/thread_count + 1;
                const I i_end   = step*(thread+1) + (corr*(thread+1))/thread_count + 1;
                
                if( i_end > i_begin )
                {
                    T s_local  = a[i_begin];
                    b[i_begin] = s_local;
                    
                    for( I i = i_begin+1; i < i_end; ++i )
                    {
                        s_local = f(s_local, a[i]);
                        b[i] = s_local;
                    }
                    S[thread+1] = s_local;
                }
            },
            thread_count
        );
        
        // scan through the last results of each chuck
        {
            T s = S[0];
            for( I i = 0; i < thread_count; ++i )
            {
                s = f(s, S[i+1]);
                S[i+1] = s;
            }
        }
        
        ParallelDo(
            [=]( const I thread )
            {
                // now each thread corrects its result
                const T correction = S[thread];
                
                const I i_begin = step*(thread  ) + (corr*(thread  ))/thread_count + 1;
                const I i_end   = step*(thread+1) + (corr*(thread+1))/thread_count + 1;
                
                for( I i = i_begin; i < i_end; ++i )
                {
                    b[i] = f(b[i], correction);
                }
            },
            thread_count
        );
    }
    
    

    
    template<typename T, typename I>
    inline void parallel_accumulate( mptr<T> a, const I n, const I thread_count )
    {
        std::vector<T> S_buffer ( static_cast<Size_T>(thread_count+1) );
        
        mptr<T> S = &S_buffer[0];
        
        S[0] = static_cast<T>(0);
        
        const I step = n / thread_count;
        const I corr = n % thread_count;
        
        
        ParallelDo(
            [=]( const I thread )
            {
                // now each thread does the scan on its chunk
                const I i_begin = step*(thread  ) + (corr*(thread  ))/thread_count;
                const I i_end   = step*(thread+1) + (corr*(thread+1))/thread_count;
                
                if( i_end > i_begin )
                {
                    T s_local = static_cast<T>(0);
                    
                    for( I i = i_begin; i < i_end; ++i )
                    {
                        s_local += a[i];
                        a[i] = s_local;
                    }
                    S[thread+1] = a[i_end-1];
                }
                else
                {
                    S[thread+1] = static_cast<T>(0);
                }
            },
            thread_count
        );
        
        // scan through the last results of each chunk
        {
            T s_local = static_cast<T>(0);
            for( I thread = 0; thread < thread_count; ++thread )
            {
                s_local += S[thread+1];
                S[thread+1] = s_local;
            }
        }
        
        ParallelDo(
            [=]( const I thread )
            {
                // now each thread corrects its result
                const T correction = S[thread];
                
                const I i_begin = step*(thread  ) + (corr*(thread  ))/thread_count;
                const I i_end   = step*(thread+1) + (corr*(thread+1))/thread_count;
                
                for( I i = i_begin; i < i_end; ++i )
                {
                    a[i] += correction;
                }
            },
            thread_count
        );
    }
    
    template<typename T, typename I>
    inline void parallel_accumulate( cptr<T> a, mptr<T> b, const I n, const I thread_count )
    {
        std::vector<T> S_buffer ( static_cast<Size_T>(thread_count+1) );
        
        mptr<T> S = &S_buffer[0];
        
        S[0] = static_cast<T>(0);
        
        const I step = n / thread_count;
        const I corr = n % thread_count;
        
        ParallelDo(
            [=]( const I thread )
            {
                // now each thread does the scan on its chunk
                const I i_begin = step*(thread  ) + (corr*(thread  ))/thread_count;
                const I i_end   = step*(thread+1) + (corr*(thread+1))/thread_count;
                
                if( i_end > i_begin )
                {
                    T s_local = static_cast<T>(0);
                    
                    for( I i = i_begin; i < i_end; ++i )
                    {
                        s_local += a[i];
                        b[i] = s_local;
                    }
                    S[thread+1] = b[i_end-1];
                }
                else
                {
                    S[thread+1] = static_cast<T>(0);
                }
            },
            thread_count
        );
        
        // scan through the last results of each chunk
        {
            T s_local = static_cast<T>(0);
            for( I thread = 0; thread < thread_count; ++thread )
            {
                s_local += S[thread+1];
                S[thread+1] = s_local;
            }
        }
        
        ParallelDo(
            [=]( const I thread )
            {
                // now each thread corrects its result
                const T correction = S[thread];
                
                const I i_begin = step*(thread  ) + (corr*(thread  ))/thread_count;
                const I i_end   = step*(thread+1) + (corr*(thread+1))/thread_count;
                
                for( I i = i_begin; i < i_end; ++i )
                {
                    b[i] += correction;
                }
            },
            thread_count
        );
    }
    
} // namespace Tools
