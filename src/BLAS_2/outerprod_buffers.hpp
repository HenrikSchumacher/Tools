#pragma once

namespace Tools
{
    template <
        Size_T M = VarSize, Size_T N = VarSize,
        Op op_x = Op::Id, Op op_y = Op::Id,
        typename x_T, typename y_T, typename z_T, typename Int = Size_T>
    TOOLS_FORCE_INLINE constexpr void outerprod_buffers(
        cptr<x_T> x,
        cptr<y_T> y,
        mptr<z_T> z,  const Int ldz,
        const Int m = static_cast<Int>(M),
        const Int n = static_cast<Int>(N)
    )
    {
        static_assert( IntQ<Int>, "" );
        
        constexpr Scalar::Flag Gen  = Scalar::Flag::Generic;
        constexpr Scalar::Flag Zero = Scalar::Flag::Zero;
        
        // TODO: For large matrices we should do this in smaller blocks.
        
        if constexpr( (op_x == Op::Id) || Scalar::RealQ<x_T> )
        {
            for( Int i = 0; i < M; ++i )
            {
                combine_buffers<N,Gen,Zero,op_y,Op::Id>(
                    x[i], y, Scalar::Zero<z_T>, &z[ldz * i], n
                );
            }
        }
        else if constexpr( op_x == Op::Conj )
        {
            for( Int i = 0; i < M; ++i )
            {
                combine_buffers<N,Gen,Zero,op_y,Op::Id>(
                    Conj(x[i]), y, Scalar::Zero<z_T>, &z[ldz * i], n
                );
            }
        }
        else
        {
            static_assert( Tools::DependentFalse<x_T>, "" );
        }
    }
        
} // namespace Tools
