#pragma once

namespace Tools
{
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename a_T, typename x_T
    >
    force_inline void scale_buffer(
        cref<a_T> a, mptr<x_T> x, const Size_T n = N, const Size_T thread_count = 1
    )
    {
        {
            #pragma float_control(precise, off)
            
            modify_buffer<Scalar::Flag::Generic,N,parQ,Op::Id>(a,x,n,thread_count);
        }
    }
    
} // namespace Tools
