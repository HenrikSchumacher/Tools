#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T
    >
    force_inline void add_to_buffer(
        cptr<x_T> x, mptr<y_T> y,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        combine_buffers<Scalar::Flag::Plus,Scalar::Flag::Plus,N,parQ,Op::Id,Op::Id>(
            Scalar::One<y_T>, x,
            Scalar::One<y_T>, y,
            n,thread_count
        );
    }
    
} // namespace Tools
