#pragma once

namespace Tools
{
    template<
        Size_T M, Size_T N,
        typename X_T, typename y_T, typename R_y_T
    >
    force_inline constexpr void rowwise_reduction_vec(
        cptr<X_T> X,  const Size_T ldX,
        mptr<y_T> y,  const Size_T ldy, R_y_T && y_reducer
    )
    {
        if constexpr ( N <= 0 )
        {
            return;
        }
        
        static_assert(M > 0,"");

        using V_T = vec_T<N,X_T>;
        
        for( Size_T i = 1; i < M; ++i )
        {
            y[ldy * i] = static_cast<y_T>(
                y_reducer( *reinterpret_cast<const V_T *>(&X[ldX * i]) )
            );
        }
    }
    
    template<
        Size_T M, Size_T N,
        typename X_T, typename y_T, typename R_y_T, typename z_T, typename R_z_T
    >
    force_inline constexpr void rowwise_reduction_vec(
        cptr<X_T> X,  const Size_T ldX,
        mptr<y_T> y,  const Size_T ldy, R_y_T && y_reducer,
        mptr<z_T> z,  const Size_T ldz, R_z_T && z_reducer
    )
    {
        if constexpr ( M <= 0 )
        {
            return;
        }
        
        static_assert(N > 0,"");

        using V_T = vec_T<N,X_T>;
        
        for( Size_T i = 1; i < M; ++i )
        {
            y[ldy * i] = static_cast<y_T>(
                y_reducer( *reinterpret_cast<const V_T *>(&X[ldX * i]) )
            );
            
            z[ldz * i] = static_cast<z_T>(
                z_reducer( *reinterpret_cast<const V_T *>(&X[ldX * i]) )
            );
        }
    }
    
    template<
        Size_T M, Size_T N,
        typename X_T, typename y_T, typename R_y_T
    >
    force_inline constexpr void rowwise_reduction(
        cptr<X_T> X,  const Size_T ldX,
        mptr<y_T> y,  const Size_T ldy, R_y_T && y_reducer
    )
    {
        if constexpr ( M <= 0 )
        {
            return;
        }

        static_assert(N > 0,"");
        
        for( Size_T i = 0; i < M; ++i )
        {
            X_T r_y = X[ldX * i + 0];
            
            for( Size_T j = 1; j < N; ++j )
            {
                r_y = y_reducer( r_y, X[ldX * i + j] );
            }
            
            y[ldy * i] = static_cast<y_T>(r_y);
        }
    }
    
    template<
        Size_T M, Size_T N, typename X_T,
        typename y_T, typename R_y_T, typename z_T, typename R_z_T
    >
    force_inline constexpr void rowwise_reduction(
        cptr<X_T> X,  const Size_T ldX,
        mptr<y_T> y,  const Size_T ldy, R_y_T && y_reducer,
        mptr<z_T> z,  const Size_T ldz, R_z_T && z_reducer
    )
    {
        if constexpr ( M <= 0 )
        {
            return;
        }

        static_assert(N > 0,"");
        
        for( Size_T i = 0; i < M; ++i )
        {
            X_T r_y = X[ldX * i + 0];;
            X_T r_z = r_y;
            
            for( Size_T j = 1; j < N; ++j )
            {
                X_T x = X[ldX * i + j];
                
                r_y = y_reducer( r_y, x );
                r_z = z_reducer( r_z, x );
            }
            
            y[ldy * i] = static_cast<y_T>(r_y);
            z[ldz *     i] = static_cast<z_T>(r_z);
        }
    }
    
} // namespace Tools
