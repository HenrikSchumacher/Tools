#pragma once

namespace Tools
{
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename up_T
    >
    force_inline void elementwise_min(
        cptr<x_T> x, cptr<y_T> y, mptr<up_T> up,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_min", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<x_T,y_T> && SameQ<x_T,up_T> && VectorizableQ<x_T> )
        {
            using V_T = vec_T<N,x_T>;
            
            *reinterpret_cast<V_T *>(up) = __builtin_elementwise_min(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(y)
            );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    up[i] = Tools::Min(x[i], y[i]);
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename lo_T
    >
    force_inline void elementwise_min_update(
        cptr<x_T> x, cptr<lo_T> lo,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_min", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<x_T,lo_T> && VectorizableQ<x_T> )
        {
            using V_T = vec_T<N,x_T>;
            
            *reinterpret_cast<V_T *>(lo) = __builtin_elementwise_min(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(lo)
            );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    lo[i] = Tools::Min(x[i], lo[i]);
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename up_T
    >
    force_inline void elementwise_max(
        cptr<x_T> x, cptr<y_T> y, mptr<up_T> up,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_max", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<x_T,y_T> && SameQ<x_T,up_T> && VectorizableQ<x_T> )
        {
            using V_T = vec_T<N,x_T>;
            
            *reinterpret_cast<V_T *>(up) = __builtin_elementwise_max(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(y)
            );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    up[i] = Tools::Max(x[i], y[i]);
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename hi_T
    >
    force_inline void elementwise_max_update(
        cptr<x_T> x, cptr<hi_T> hi,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_max", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<x_T,hi_T> && VectorizableQ<x_T> )
        {
            using V_T = vec_T<N,x_T>;
            
            *reinterpret_cast<V_T *>(hi) = __builtin_elementwise_max(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(hi)
            );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    hi[i] = Tools::Max(x[i], hi[i]);
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename lo_T, typename hi_T
    >
    force_inline void elementwise_minmax(
        cptr<x_T> x, cptr<y_T> y, mptr<lo_T> lo, mptr<hi_T> up,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_minmax", thread_count );

        
        if constexpr ( (N > VarSize) && SameQ<x_T,y_T> && SameQ<x_T,lo_T> && SameQ<x_T,hi_T> && VectorizableQ<x_T> )
        {
            using V_T = vec_T<N,x_T>;
            
            *reinterpret_cast<V_T *>(lo) = __builtin_elementwise_min(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(y)
            );
            
            *reinterpret_cast<V_T *>(up) = __builtin_elementwise_max(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(y)
            );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    std::tie( lo[i], up[i] ) = Tools::MinMax(x[i], y[i]);
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename lo_T, typename up_T
    >
    force_inline void elementwise_minmax_update(
        cptr<x_T> x, mptr<lo_T> lo, mptr<up_T> up,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_minmax", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<x_T,lo_T> && SameQ<x_T,up_T> && VectorizableQ<x_T> )
        {
            using V_T = vec_T<N,x_T>;
            
            *reinterpret_cast<V_T *>(lo) = __builtin_elementwise_min(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(lo)
            );
            
            *reinterpret_cast<V_T *>(up) = __builtin_elementwise_max(
                *reinterpret_cast<const V_T *>(x),
                *reinterpret_cast<const V_T *>(up)
            );
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    lo[i] = Tools::Min( x[i], lo[i] );
                    up[i] = Tools::Max( x[i], up[i] );
                },
                n, thread_count
            );
        }
    }

} // namespace Tools
