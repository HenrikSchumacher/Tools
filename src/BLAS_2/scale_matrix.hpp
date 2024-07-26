#pragma once

namespace Tools
{
    template <
        Size_T M, Size_T N, Parallel_T parQ,
        typename b_T, typename Y_T
    >
    force_inline constexpr void scale_matrix(
        const b_T b_, mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        const auto b = scalar_cast<Y_T>(b_);
        
        if( (ldY == n) )
        {
            // Consecutive buffer. Do a standard scale_buffer.
            if constexpr ( (M > VarSize) && (N > VarSize ) )
            {
                scale_buffer<M * N,Sequential>( b, Y );
            }
            else
            {
                scale_buffer<VarSize,Parallel>( b, Y, m * n, thread_count );
            }
        }
        else
        {
            Do<M,parQ,Static>(
                [=]( const Size_T i )
                {
                    scale_buffer<N,Sequential>( b, &Y[ldY * i], n );
                },
                m, thread_count
            );
        }
    }
    
    
    // Overload for fixed sizes.
    template <
        Size_T M, Size_T N,
        typename b_T, typename Y_T
    >
    force_inline constexpr void scale_matrix(
        const b_T b, mptr<Y_T> Y, const Size_T ldY
    )
    {
        static_assert( M > VarSize, "" );
        static_assert( N > VarSize, "" );
        
        scale_matrix<M,N,Sequential>( b, Y, ldY, M, N, Size_T(1) );
    }
    
    
    // Overload for variable sizes, parallel evaluation.
    template <
        typename b_T, typename Y_T
    >
    force_inline constexpr void scale_matrix(
        const b_T b, mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n, const Size_T thread_count
    )
    {
        scale_matrix<VarSize,VarSize,Parallel>( b, Y, ldY, m, n, thread_count );
    }
    
    // Overload for variable sizes, sequential evaluation.
    template <
        typename b_T, typename Y_T
    >
    force_inline constexpr void scale_matrix(
        const b_T b, mptr<Y_T> Y, const Size_T ldY,
        const Size_T m, const Size_T n
    )
    {
        scale_matrix<VarSize,VarSize,Sequential>( b, Y, ldY, m, n, Size_T(1) );
    }
    
} // namespace Tools


