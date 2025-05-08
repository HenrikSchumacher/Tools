#pragma once

namespace Tools
{
    template<
    Size_T M, Size_T N,
        typename T, typename Fun,
        typename Int = Size_T
    >
    void row_reverse_matrix(
        mptr<T> A,
        const Int ldA,
        mptr<T> scratch,
        const Int m,
        const Int n,
        Fun && fun
    )
    {
        static_assert(IntQ<Int>, "");
        
        if constexpr ( M > VarSize )
        {
            const Int half = M/2;
            
            for( Int i = 0; i < half; ++i )
            {
                copy_buffer<N>( &A[ldA * i], scratch, n );
                fun( &A[ldA *(m-i-1)], &A[ldA * i]      );
                fun( scratch         , &A[ldA *(m-i-1)] );
            }
            
            if( M % 2 )
            {
                copy_buffer<N>( &A[ldA * half], scratch, n );
                fun( scratch, &A[ldA * half] );
            }
        }
        else
        {
            const Int half = m/2;
            
            for( Int i = 0; i < half; ++i )
            {
                copy_buffer<N>( &A[ldA * i], scratch, n );
                fun( &A[ldA *(m-i-1)], &A[ldA * i]      );
                fun( scratch         , &A[ldA *(m-i-1)] );
            }
            
            if( m % 2 )
            {
                copy_buffer<N>( &A[ldA * half], scratch, n );
                fun( scratch, &A[ldA * half] );
            }
        }
    }
    
    template<Size_T M, Size_T N, typename T, typename Int = Size_T>
    void row_reverse_matrix(
        mptr<T> A,
        const Int ldA,
        const Int m = static_cast<Int>(M),
        const Int n = static_cast<Int>(N)
    )
    {
        static_assert(IntQ<Int>, "");
        T * scratch = nullptr;
        
        auto fun = [n]( cptr<T> from, mptr<T> to )
        {
            copy_buffer<N>( from, to, n );
        };
        
        safe_alloc( scratch, n );
        
        row_reverse_matrix<M,N>( A, ldA, scratch, m, n, std::move(fun) );
        
        safe_free( scratch );
    }
    

    
    template<Size_T M, Size_T N, Side dir,
        typename T, typename PreScan, typename PostScan,
        typename Int = Size_T
    >
    void row_rotate_matrix(
        mptr<T> A,
        const Int ldA,
        const Int shift,
        const Int m,
        const Int n,
        PreScan  && pre_scan,
        PostScan && post_scan
    )
    {
        static_assert(IntQ<Int>, "");
        
        if( m <= Int(0) )
        {
            return;
        }
        
        Int s = (dir == Side::Left) ? (shift % m) : (-shift % m);
        
        if( s < Int(0) )
        {
            s += m;
        }
  
        // We must not abort here because pre_scan and post_scan need to be applied.
//        if( s == Int(0) )
//        {
//            return;
//        }
        
        const Int k = m - s;
        
        T * scratch = nullptr;
        safe_alloc( scratch, ToSize_T(n) );
        
        row_reverse_matrix<M,N>( &A[ldA * 0], ldA, scratch, s, n, pre_scan  );
        row_reverse_matrix<M,N>( &A[ldA * s], ldA, scratch, k, n, pre_scan  );
        row_reverse_matrix<M,N>( &A[ldA * 0], ldA, scratch, m, n, post_scan );
            
        safe_free( scratch );
    }
    
    
    template<Size_T M, Size_T N, Side dir, typename T, typename Int = Size_T >
    void row_rotate_matrix(
        mptr<T> A,
        const Int ldA,
        const Int shift,
        const Int m = static_cast<Int>(M),
        const Int n = static_cast<Int>(N)
    )
    {
        if( shift / m == Int(0) )
        {
            return;
        }
        
        auto fun = [n]( cptr<T> from, mptr<T> to )
        {
            copy_buffer<N>( from, to, n );
        };
        
        row_rotate_matrix<M,N,dir>( A, ldA, shift, m, n, std::move(fun), std::move(fun) );
    }
    
} // namespace Tools
