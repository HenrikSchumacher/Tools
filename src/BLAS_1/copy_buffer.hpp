#pragma once

namespace Tools
{
    
    template<typename From, typename To>
    struct static_caster
    {
        constexpr To operator()(const From & p)
        {
            return static_cast<To>(p);
        }
    };
    
    
    // This is a bit weird: Typically this fixed-size version should not improve anything because clang and gcc do some heavy optimization for std::copy. In a nutshell, they analyze in every call to std::copy whether n is a compile-time constant -- something that one would not expect a compiler to do for a non-template parameter! However, godbolt tells me that this only happens with at least one restrict qualifier; otherwise the call to std::copy is redirected to std::memmove.
    // We add this fixed-size version anyways so that we do not have to rely on this somewhat unexpected compiler optimization.
    
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename S, typename T, typename Int = Size_T
    >
    TOOLS_FORCE_INLINE constexpr void copy_buffer(
        cptr<S> from, mptr<T> to,
        const Int n = static_cast<Int>(N),
        const Int thread_count = Int(1)
    )
    {
        check_sequential<parQ>( "copy_buffer", thread_count );
        
        static_assert(IntQ<Int>,"");
        
        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Sequential )
            {
                
                if constexpr ( SameQ<T,S> )
                {
                    std::copy( &from[0], &from[n], &to[0] );
                }
                else
                {
                    std::transform( &from[0], &from[n], &to[0], static_caster<S,T>() );
                }
            }
            else
            {
                if( thread_count <= Scalar::One<Int> )
                {
                    if constexpr ( SameQ<T,S> )
                    {
                        std::copy( &from[0], &from[n], &to[0] );
                    }
                    else
                    {
                        std::transform( &from[0], &from[n], &to[0], static_caster<S,T>() );
                    }
                }
                else
                {
                    ParallelDo(
                        [=]( const Int thread )
                        {
                            const Int begin = JobPointer(n,thread_count,thread  );
                            const Int end   = JobPointer(n,thread_count,thread+1);

                            if constexpr ( SameQ<T,S> )
                            {
                                std::copy( &from[begin], &from[end], &to[begin] );
                            }
                            else
                            {
                                std::transform( &from[begin], &from[end], &to[begin], static_caster<S,T>() );
                            }
                        },
                        thread_count
                    );
                }
            }
        }
        else
        {
            if constexpr ( SameQ<T,S> )
            {
                std::copy( &from[0], &from[N], &to[0] );
            }
            else
            {
                std::transform( &from[0], &from[N], &to[0], static_caster<S,T>() );
            }
        }
        
    }
    
} // namespace Tools
