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
        const T c = std::min(a,b);
        b = std::max(a,b);
        a = c;
    }
    
//    template<typename T>
//    force_inline void Sort2( T & a, T & b)
//    {
//        auto p = std::minmax(a,b);
//
//        a = p.first;
//        b = p.second;
//    }
    

    
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
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct Sorter<5,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            
            Sort2(a[1],a[3]);
            Sort2(a[2],a[4]);
            
            Sort2(a[0],a[2]);
            Sort2(a[1],a[4]);
            
            Sort2(a[1],a[2]);
            Sort2(a[3],a[4]);
            
            Sort2(a[2],a[3]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct Sorter<6,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            Sort2(a[4],a[5]);
            
            Sort2(a[0],a[2]);
            Sort2(a[3],a[5]);
            Sort2(a[1],a[4]);
            
            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            Sort2(a[4],a[5]);
            
            Sort2(a[1],a[2]);
            Sort2(a[3],a[4]);

            Sort2(a[2],a[3]);
        }
    };
    
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct Sorter<7,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[1],a[2]);
            Sort2(a[3],a[4]);
            Sort2(a[5],a[6]);

            Sort2(a[0],a[2]);
            Sort2(a[4],a[6]);
            Sort2(a[3],a[5]);

            Sort2(a[2],a[6]);
            Sort2(a[1],a[5]);
            Sort2(a[0],a[4]);

            Sort2(a[2],a[5]);
            Sort2(a[0],a[3]);

            Sort2(a[2],a[4]);
            Sort2(a[1],a[3]);

            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            Sort2(a[4],a[5]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct Sorter<8,T>
    {
        void operator()( mut<T> a )
        {
            Sort2(a[0],a[2]);
            Sort2(a[1],a[3]);
            Sort2(a[4],a[6]);
            Sort2(a[5],a[7]);
            
            Sort2(a[0],a[4]);
            Sort2(a[1],a[5]);
            Sort2(a[2],a[6]);
            Sort2(a[3],a[7]);
            
            Sort2(a[0],a[1]);
            Sort2(a[2],a[3]);
            Sort2(a[4],a[5]);
            Sort2(a[6],a[7]);
            
            Sort2(a[2],a[4]);
            Sort2(a[3],a[5]);
            
            Sort2(a[1],a[4]);
            Sort2(a[3],a[6]);
            
            Sort2(a[1],a[2]);
            Sort2(a[3],a[4]);
            Sort2(a[5],a[6]);
            
        }
    };
}
