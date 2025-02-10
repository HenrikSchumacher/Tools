#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename Int = Size_T
    >
    force_inline void add_to_buffer(
        cptr<x_T> x, mptr<y_T> y,
        const Int n = N, const Int thread_count = 1
    )
    {
        static_assert( IntQ<Int>, "" );
        
        combine_buffers<Scalar::Flag::Plus,Scalar::Flag::Plus,N,parQ,Op::Id,Op::Id>(
            Scalar::One<y_T>, x,
            Scalar::One<y_T>, y,
            n,thread_count
        );
    }
    
} // namespace Tools
