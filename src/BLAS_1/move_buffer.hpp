#pragma once

namespace Tools
{
    
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename T, typename Int = Size_T
    >
    force_inline void move_buffer(
        cptr<T> from, mptr<T> to, const Int n = static_cast<Int>(N), const Int thread_count = 1
    )
    {
        check_sequential<parQ>( "move_buffer", thread_count );
        
        static_assert( IntQ<Int>, "");
        
        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Sequential )
            {
                
                std::memmove( &to[0], &from[0], n );
            }
            else
            {
                if( thread_count <= Int(1) )
                {
                    std::memmove( &to[0], &from[0], n );
                }
                else
                {
                    ParallelDo(
                        [=]( const Int thread )
                        {
                            const Int begin = JobPointer(n,thread_count,thread  );
                            const Int end   = JobPointer(n,thread_count,thread+1);

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
