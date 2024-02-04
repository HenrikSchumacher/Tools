#pragma once

namespace Tools
{
    namespace Zippers
    {
        template <typename T_a, typename T_b, typename T_c>
        struct Plus
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = a + b;
            }
        };
        
        template <typename T_a, typename T_b, typename T_c>
        struct Minus
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = a - b;
            }
        };
        
        template <typename T_a, typename T_b, typename T_c>
        struct Times
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = a * b;
            }
        };
        
        template <typename T_a, typename T_b, typename T_c>
        struct Divide
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = a / b;
            }
        };
        
        template <typename T_a, typename T_b, typename T_c>
        struct Max
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = Max(a,b);
            }
        };
        
        template <typename T_a, typename T_b, typename T_c>
        struct Min
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = Min(a,b);
            }
        };
        
    }
    
    template <
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        typename F, typename T_a, typename T_b, typename T_c
    >
    force_inline void zip_buffers(
        F && zip, cptr<T_a> a, cptr<T_b> b, mptr<T_c> c,
        const Size_T n = N, const Size_T thread_count = 1
    )
    {
        check_sequential<parQ>( "zip_buffers", thread_count );
        
        Do<N,parQ>(
            [=]( const Size_T i )
            {
                zip( a[i], b[i], c[i] );
            },
            n, thread_count
        );
    }
    
} // namespace Tools
