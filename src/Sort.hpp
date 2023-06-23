#pragma once

namespace Tools
{
    template<int n, typename T>
    struct Sorter
    {
        void operator()( mut<T> a )
        {
            std::sort( a, a+n );
        }
    };
    
    template<typename T>
    force_inline void Sort2( T & a, T & b)
    {
        using std::swap;
        
        if( a > b )
        {
            swap(a,b);
        }
    }
    
    template<typename T>
    struct Sorter<1,T>
    {
        void operator()( mut<T> a )
        {}
    };
    
    template<typename T>
    struct Sorter<2,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[0],a[1]);
        }
    };
    
    template<typename T>
    struct Sorter<3,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[0],a[1]);
            Sort2(a[0],a[2]);
            Sort2(a[1],a[2]);
        }
    };
    
    template<typename T>
    struct Sorter<4,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            
            Sort2(a[0],a[2]);
            Sort2(a[1],a[3]);
            
            Sort2(a[1],a[2]);
        }
    };
    
    template<typename T>
    struct Sorter<5,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[1],a[2]);
            Sort2(a[3],a[4]);
            
            Sort2(a[0],a[2]);
            Sort2(a[1],a[3]);
            
            Sort2(a[0],a[3]);
            Sort2(a[2],a[4]);
            
            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            
            Sort2(a[1],a[2]);
        }
    };
}
