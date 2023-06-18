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
                return c = std::max(a,b);
            }
        };
        
        template <typename T_a, typename T_b, typename T_c>
        struct Min
        {
            void operator()( const T_a & a, const T_b & b, T_c & c )
            {
                return c = std::min(a,b);
            }
        };
        
    }
    
    
    template <typename F, typename T_a, typename T_b, typename T_c>
    force_inline void zip_buffers(
        F & zip, ptr<T_a> a, ptr<T_b> b, mut<T_c> c, const Size_T n
    )
    {
        for( Size_T i = 0; i < n; ++i )
        {
            zip( a[i], b[i], c[i] );
        }
    }
    
    template <typename F, typename T_a, typename T_b, typename T_c>
    force_inline void zip_buffers(
        F & zip, ptr<T_a> a, ptr<T_b> b, mut<T_c> c, const Size_T n, const Size_T thread_count
    )
    {
        ParallelDo(
            [=]( const Size_T i )
            {
                zip( a[i], b[i], c[i] );
            },
            n,
            thread_count
        );
    }
    
    template <Size_T n, typename F, typename T_a, typename T_b, typename T_c>
    force_inline void zip_buffers(
        F & zip, ptr<T_a> a, ptr<T_b> b, mut<T_c> c
    )
    {
        for( Size_T i = 0; i < n; ++i )
        {
            zip( a[i], b[i], c[i] );
        }
    }
    
} // namespace Tools
