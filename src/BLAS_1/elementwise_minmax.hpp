#pragma once

namespace Tools
{
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename z_T, typename Int = Size_T
    >
    TOOLS_FORCE_INLINE void elementwise_min(
        cptr<x_T> x, cptr<y_T> y, mptr<z_T> z,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_min", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<y_T,z_T> && SameQ<x_T,z_T> && VectorizableQ<z_T> )
        {
            using V_T = vec_T<N,z_T>;
            
            V_T x_vec;
            V_T y_vec;
            
            copy_buffer<N>(x,reinterpret_cast<z_T*>(&x_vec));
            copy_buffer<N>(y,reinterpret_cast<z_T*>(&y_vec));
            
            V_T z_vec = __builtin_elementwise_min(x_vec,y_vec);
            
            copy_buffer<N>(reinterpret_cast<z_T*>(&z_vec),z);
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    z[i] = Tools::Min(
                        static_cast<z_T>(x[i]),
                        static_cast<z_T>(y[i])
                    );
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename z_T
    >
    TOOLS_FORCE_INLINE void elementwise_min_update(
        cptr<x_T> x, mptr<z_T> lo,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_min", thread_count );
        
        if constexpr ( (N > VarSize) /*&& SameQ<x_T,lo_T>*/ && VectorizableQ<z_T> )
        {
            using V_T = vec_T<N,z_T>;
            
            V_T x_vec;
            V_T y_vec;
            
            copy_buffer<N>(x ,reinterpret_cast<z_T*>(&x_vec));
            copy_buffer<N>(lo,reinterpret_cast<z_T*>(&y_vec));
            
            V_T z_vec = __builtin_elementwise_min(x_vec,y_vec);
            
            copy_buffer<N>(reinterpret_cast<z_T*>(&z_vec),lo);
            
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    lo[i] = Tools::Min(static_cast<z_T>(x[i]),lo[i]);
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename z_T
    >
    TOOLS_FORCE_INLINE void elementwise_max(
        cptr<x_T> x, cptr<y_T> y, mptr<z_T> z,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_max", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<y_T,z_T> && SameQ<x_T,z_T> && VectorizableQ<z_T> )
        {
            using V_T = vec_T<N,z_T>;
            
            V_T x_vec;
            V_T y_vec;
            
            copy_buffer<N>(x,reinterpret_cast<z_T*>(&x_vec));
            copy_buffer<N>(y,reinterpret_cast<z_T*>(&y_vec));
            
            V_T z_vec = __builtin_elementwise_max(x_vec,y_vec);
            
            copy_buffer<N>(reinterpret_cast<z_T*>(&z_vec),z);
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    z[i] = Tools::Max(
                        static_cast<z_T>(x[i]),
                        static_cast<z_T>(y[i])
                    );
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename z_T
    >
    TOOLS_FORCE_INLINE void elementwise_max_update(
        cptr<x_T> x, mptr<z_T> z,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_max", thread_count );
        
        if constexpr ( (N > VarSize) && SameQ<x_T,z_T> && VectorizableQ<z_T> )
        {
            using V_T = vec_T<N,z_T>;
            
            V_T x_vec;
            V_T y_vec;
            
            copy_buffer<N>(x,reinterpret_cast<z_T*>(&x_vec));
            copy_buffer<N>(z,reinterpret_cast<z_T*>(&y_vec));
            
            V_T z_vec = __builtin_elementwise_max(x_vec,y_vec);
            
            copy_buffer<N>(reinterpret_cast<z_T*>(&z_vec),z);
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    z[i] = Tools::Max( static_cast<z_T>(x[i]),z[i] );
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename y_T, typename z_T
    >
    TOOLS_FORCE_INLINE void elementwise_minmax(
        cptr<x_T> x, cptr<y_T> y, mptr<z_T> lo, mptr<z_T> hi,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_minmax", thread_count );

        
        if constexpr ( (N > VarSize) && VectorizableQ<z_T> )
        {
            using V_T = vec_T<N,z_T>;
            
            V_T x_vec;
            V_T y_vec;
            
            copy_buffer<N>(x,reinterpret_cast<z_T*>(&x_vec));
            copy_buffer<N>(y,reinterpret_cast<z_T*>(&y_vec));
            
            V_T lo_vec = __builtin_elementwise_min(x_vec,y_vec);
            V_T hi_vec = __builtin_elementwise_max(x_vec,y_vec);
            
            copy_buffer<N>(reinterpret_cast<z_T*>(&lo_vec),lo);
            copy_buffer<N>(reinterpret_cast<z_T*>(&hi_vec),hi);
        }
        else
        {
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    std::tie(lo[i],hi[i]) = Tools::MinMax(
                        static_cast<z_T>(x[i]),
                        static_cast<z_T>(y[i])
                    );
                },
                n, thread_count
            );
        }
    }
    
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename x_T, typename z_T
    >
    TOOLS_FORCE_INLINE void elementwise_minmax_update(
        cptr<x_T> x, mptr<z_T> lo, mptr<z_T> hi,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        using namespace Scalar;
        
        check_sequential<parQ>( "elementwise_minmax", thread_count );
        
        if constexpr ( (N > VarSize) /*&& SameQ<x_T,z_T>*/ && VectorizableQ<z_T> )
        {
            using V_T = vec_T<N,z_T>;
            
            V_T x_vec;
            V_T lo_vec;
            V_T hi_vec;
            
            copy_buffer<N>(x,reinterpret_cast<z_T*>(&x_vec));
            
            lo_vec = __builtin_elementwise_min(x_vec,lo_vec);
            hi_vec = __builtin_elementwise_max(x_vec,hi_vec);
            
            copy_buffer<N>(reinterpret_cast<z_T*>(&lo_vec),lo);
            copy_buffer<N>(reinterpret_cast<z_T*>(&hi_vec),hi);
        }
        else
        {
            TOOLS_MAKE_FP_FAST()
            
            Do<N,parQ>(
                [=]( const Size_T i )
                {
                    lo[i] = Tools::Min(static_cast<z_T>(x[i]),lo[i]);
                    hi[i] = Tools::Max(static_cast<z_T>(x[i]),hi[i]);
                },
                n, thread_count
            );
        }
    }

} // namespace Tools
