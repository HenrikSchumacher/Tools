#pragma once

namespace Tools
{
    template<
        Size_T M, Size_T N,
        typename X_T, typename y_T, typename R_y_T
    >
    force_inline constexpr void columnwise_reduction_vec(
        cptr<X_T> X,  const Size_T ldX,
        mptr<y_T> y,  const Size_T ldy, R_y_T && y_reducer
    )
    {
        if constexpr ( M <= 0 )
        {
            return;
        }
        
        static_assert(N > 0,"");

        using V_T = vec_T<N,X_T>;
        
        V_T y_vec = *reinterpret_cast<const V_T *>(X);
        
        for( Size_T i = 1; i < M; ++i )
        {
            y_vec = y_reducer( y_vec, *reinterpret_cast<const V_T *>(&X[ldX * i]) );
        }
        
        copy_matrix<N,1>( reinterpret_cast<X_T *>(&y_vec), Size_T(1), y, ldy );
    }
    
    template<
        Size_T M, Size_T N,
        typename X_T, typename y_T, typename R_y_T, typename z_T, typename R_z_T
    >
    force_inline constexpr void columnwise_reduction_vec(
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
        
        V_T y_vec = *reinterpret_cast<const V_T *>(X);
        V_T z_vec = *reinterpret_cast<const V_T *>(X);
        
        for( Size_T i = 1; i < M; ++i )
        {
            y_vec = y_reducer( y_vec, *reinterpret_cast<const V_T *>(&X[ldX * i]) );
            z_vec = z_reducer( z_vec, *reinterpret_cast<const V_T *>(&X[ldX * i]) );
        }
        
        copy_matrix<N,1>( reinterpret_cast<X_T *>(&y_vec), Size_T(1), y, ldy );
        copy_matrix<N,1>( reinterpret_cast<X_T *>(&z_vec), Size_T(1), z, ldz );
    }
    
    template<
        Size_T M, Size_T N,
        typename X_T, typename y_T, typename R_y_T
    >
    force_inline constexpr void columnwise_reduction(
        cptr<X_T> X,  const Size_T ldX,
        mptr<y_T> y,  const Size_T ldy, R_y_T && y_reducer
    )
    {
        
        if constexpr ( M <= 0 )
        {
            return;
        }

        static_assert(N > 0,"");
        
        X_T y_buf [N];
        
        copy_buffer<N>( X, &y_buf[0] );
        
        for( Size_T i = 1; i < M; ++i )
        {
            for( Size_T j = 0; j < N; ++j )
            {
                y_buf[j] = y_reducer( y_buf[j], X[ldX * i + j] );
            }
        }
        
        copy_matrix<N,1>( &y_buf[0], Size_T(1), y, ldy );
    }
    
    template<
        Size_T M, Size_T N, typename X_T,
        typename y_T, typename R_y_T, typename z_T, typename R_z_T
    >
    force_inline constexpr void columnwise_reduction(
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

        X_T y_buf [N];
        X_T z_buf [N];
        
        copy_buffer<N>( X, &y_buf[0] );
        copy_buffer<N>( X, &z_buf[0] );
        
        for( Size_T i = 1; i < M; ++i )
        {
            for( Size_T j = 0; j < N; ++j )
            {
                y_buf[j] = y_reducer( y_buf[j], X[ldX * i + j] );
                z_buf[j] = z_reducer( z_buf[j], X[ldX * i + j] );
            }
        }
        
        copy_matrix<N,1>( &y_buf[0], Size_T(1), y, ldy );
        copy_matrix<N,1>( &z_buf[0], Size_T(1), z, ldz );
    }
    
} // namespace Tools
