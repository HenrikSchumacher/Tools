#pragma once

namespace Tools
{
    
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T
    >
    force_inline void move_buffer(
        const T * from, T * to, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "move_buffer", thread_count );
        
        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Sequential )
            {
                
                std::memmove( &to[0], &from[0], n );
            }
            else
            {
                if( thread_count <= Scalar::One<Size_T> )
                {
                    std::memmove( &to[0], &from[0], n );
                }
                else
                {
                    ParallelDo(
                        [=]( const Size_T thread )
                        {
                            const Size_T begin = JobPointer(n,thread_count,thread  );
                            const Size_T end   = JobPointer(n,thread_count,thread+1);

                            std::memmove( &to[begin], &from[begin], end-begin );
                        },
                        thread_count
                    );
                }
            }
        }
        else
        {
            std::memmove( &to[0], &from[0], n );
        }
    }
    
} // namespace Tools
