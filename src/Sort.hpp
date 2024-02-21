#pragma once

#include <functional>

namespace Tools
{
    
    template<typename T, typename C = std::less<T>>
    force_inline void CompSwap( mref<T> a, mref<T> b, C comp = C()  )
    {
        // Beware, std::tie( a, b ) = std::minmax(a,b,comp) does not work!!!

        // The initializer list makes this fast. I don't know why!
        std::tie(a,b) = std::minmax({a,b},comp); // We need to enforce a copy.
        
//        // This 3-4 times slower. I don't really have any idea why:
//        const std::pair<T,T> p = std::minmax(a,b,comp);
//        a = p.first;
//        b = p.second;

        
//        // Even slower:
//        if( comp(b,a) )
//        {
//            std::swap(a,b);
//        }
    }
    
    
    template<int n, typename T>
    struct SortNet
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer( a, b, n );
            this->operator()( b, comp );
        }
        
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            std::sort( a, a+n, comp );
        }
    };

    template<typename T>
    struct SortNet<1,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<1>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {}
    };
    
    template<typename T>
    struct SortNet<2,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<2>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[1],comp);
        }
    };
    
    template<typename T>
    struct SortNet<3,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<3>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[1],comp);
            CompSwap(a[0],a[2],comp);
            CompSwap(a[1],a[2],comp);
        }
    };
    
    template<typename T>
    struct SortNet<4,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<4>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            
            CompSwap(a[0],a[2],comp);
            CompSwap(a[1],a[3],comp);
            
            CompSwap(a[1],a[2],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<5,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<5>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            
            CompSwap(a[1],a[3],comp);
            CompSwap(a[2],a[4],comp);
            
            CompSwap(a[0],a[2],comp);
            CompSwap(a[1],a[4],comp);
            
            CompSwap(a[1],a[2],comp);
            CompSwap(a[3],a[4],comp);
            
            CompSwap(a[2],a[3],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<6,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<6>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
            
            CompSwap(a[0],a[2],comp);
            CompSwap(a[3],a[5],comp);
            CompSwap(a[1],a[4],comp);
            
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
            
            CompSwap(a[1],a[2],comp);
            CompSwap(a[3],a[4],comp);

            CompSwap(a[2],a[3],comp);
        }
    };
    
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<7,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<7>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[1],a[2],comp);
            CompSwap(a[3],a[4],comp);
            CompSwap(a[5],a[6],comp);

            CompSwap(a[0],a[2],comp);
            CompSwap(a[4],a[6],comp);
            CompSwap(a[3],a[5],comp);

            CompSwap(a[2],a[6],comp);
            CompSwap(a[1],a[5],comp);
            CompSwap(a[0],a[4],comp);

            CompSwap(a[2],a[5],comp);
            CompSwap(a[0],a[3],comp);

            CompSwap(a[2],a[4],comp);
            CompSwap(a[1],a[3],comp);

            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<8,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<8>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[2],comp);
            CompSwap(a[1],a[3],comp);
            CompSwap(a[4],a[6],comp);
            CompSwap(a[5],a[7],comp);
            
            CompSwap(a[0],a[4],comp);
            CompSwap(a[1],a[5],comp);
            CompSwap(a[2],a[6],comp);
            CompSwap(a[3],a[7],comp);
            
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
            CompSwap(a[6],a[7],comp);
            
            CompSwap(a[2],a[4],comp);
            CompSwap(a[3],a[5],comp);
            
            CompSwap(a[1],a[4],comp);
            CompSwap(a[3],a[6],comp);
            
            CompSwap(a[1],a[2],comp);
            CompSwap(a[3],a[4],comp);
            CompSwap(a[5],a[6],comp);
            
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<9,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<9>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[1],a[8],comp);
            CompSwap(a[2],a[7],comp);
            CompSwap(a[3],a[6],comp);
            CompSwap(a[4],a[5],comp);
            
            CompSwap(a[1],a[4],comp);
            CompSwap(a[0],a[2],comp);
            CompSwap(a[5],a[8],comp);
            CompSwap(a[6],a[7],comp);
            
            CompSwap(a[2],a[6],comp);
            CompSwap(a[7],a[8],comp);
            CompSwap(a[0],a[3],comp);
            CompSwap(a[4],a[5],comp);
            
            CompSwap(a[0],a[1],comp);
            CompSwap(a[3],a[5],comp);
            CompSwap(a[6],a[7],comp);
            CompSwap(a[2],a[4],comp);
            
            CompSwap(a[1],a[3],comp);
            CompSwap(a[5],a[7],comp);
            CompSwap(a[4],a[6],comp);
            
            CompSwap(a[1],a[2],comp);
            CompSwap(a[3],a[4],comp);
            CompSwap(a[5],a[6],comp);
            CompSwap(a[7],a[8],comp);
            
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<10,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<10>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
            CompSwap(a[6],a[7],comp);
            CompSwap(a[8],a[9],comp);
            
            CompSwap(a[4],a[9],comp);
            CompSwap(a[0],a[5],comp);
            CompSwap(a[1],a[8],comp);
            CompSwap(a[3],a[7],comp);
            CompSwap(a[2],a[6],comp);
            
            CompSwap(a[0],a[2],comp);
            CompSwap(a[3],a[6],comp);
            CompSwap(a[7],a[9],comp);
            CompSwap(a[1],a[4],comp);
            CompSwap(a[5],a[8],comp);
            
            CompSwap(a[0],a[1],comp);
            CompSwap(a[2],a[7],comp);
            CompSwap(a[8],a[9],comp);
            CompSwap(a[4],a[6],comp);
            CompSwap(a[3],a[5],comp);
            
            CompSwap(a[2],a[4],comp);
            CompSwap(a[6],a[8],comp);
            CompSwap(a[1],a[3],comp);
            CompSwap(a[5],a[7],comp);
            
            CompSwap(a[1],a[2],comp);
            CompSwap(a[3],a[4],comp);
            CompSwap(a[5],a[6],comp);
            CompSwap(a[7],a[8],comp);
            
            CompSwap(a[2],a[3],comp);
            CompSwap(a[4],a[5],comp);
            CompSwap(a[6],a[7],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<11,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<11>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[ 0],a[ 9],comp);
            CompSwap(a[ 1],a[ 8],comp);
            CompSwap(a[ 2],a[ 7],comp);
            CompSwap(a[ 3],a[ 6],comp);
            CompSwap(a[ 4],a[ 5],comp);
            
            CompSwap(a[ 0],a[ 3],comp);
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 4],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[ 9],a[10],comp);
            
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 4],a[ 6],comp);
            CompSwap(a[ 8],a[10],comp);
            CompSwap(a[ 5],a[ 9],comp);
            
            CompSwap(a[ 0],a[ 4],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[ 1],a[ 5],comp);
            CompSwap(a[ 2],a[ 9],comp);
            CompSwap(a[ 3],a[ 6],comp);
            
            CompSwap(a[ 1],a[ 4],comp);
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 5],a[ 7],comp);
            CompSwap(a[ 6],a[ 9],comp);
            
            CompSwap(a[ 2],a[ 4],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[ 8],a[ 9],comp);
            CompSwap(a[ 3],a[ 5],comp);
            
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 7],a[ 8],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<12,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<12>( a, b );
            this->operator()( b, comp );
        }
     
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[ 0],a[ 6],comp);
            CompSwap(a[ 1],a[ 7],comp);
            CompSwap(a[ 2],a[ 8],comp);
            CompSwap(a[ 3],a[ 9],comp);
            CompSwap(a[ 4],a[10],comp);
            CompSwap(a[ 5],a[11],comp);
            
            CompSwap(a[ 0],a[ 3],comp);
            CompSwap(a[ 1],a[ 4],comp);
            CompSwap(a[ 2],a[ 5],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[10],comp);
            CompSwap(a[ 8],a[11],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[10],a[11],comp);
            
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 3],a[ 6],comp);
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[ 9],a[10],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 2],a[ 9],comp);
            CompSwap(a[10],a[11],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[ 6],a[ 7],comp);
            
            CompSwap(a[ 1],a[ 3],comp);
            CompSwap(a[ 4],a[ 7],comp);
            CompSwap(a[ 8],a[10],comp);
            CompSwap(a[ 2],a[ 6],comp);
            CompSwap(a[ 5],a[ 9],comp);
            
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 4],a[ 6],comp);
            CompSwap(a[ 8],a[ 9],comp);
            CompSwap(a[ 5],a[ 7],comp);
            
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 7],a[ 8],comp);
        }
    };
   
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<13,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<13>( a, b );
            this->operator()( b, comp );
        }
     
        // This should be a depth-optimal solution.
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[ 3],a[12],comp);
            CompSwap(a[ 4],a[11],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            
            CompSwap(a[ 0],a[ 7],comp);
            CompSwap(a[ 1],a[ 6],comp);
            CompSwap(a[ 2],a[ 5],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[11],a[12],comp);
            
            CompSwap(a[ 0],a[ 3],comp);
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 4],a[ 7],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 8],a[11],comp);
            CompSwap(a[ 9],a[10],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 2],a[ 8],comp);
            CompSwap(a[10],a[11],comp);
            CompSwap(a[ 3],a[ 9],comp);
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[12],comp);
            
            CompSwap(a[ 1],a[ 4],comp);
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[11],a[12],comp);
            CompSwap(a[ 6],a[10],comp);
            CompSwap(a[ 7],a[ 9],comp);
            
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 4],a[ 6],comp);
            CompSwap(a[ 9],a[11],comp);
            CompSwap(a[ 5],a[ 7],comp);
            CompSwap(a[ 8],a[10],comp);
            
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[ 8],a[ 9],comp);
            CompSwap(a[10],a[11],comp);
            
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[ 9],a[10],comp);
            CompSwap(a[11],a[12],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<14,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<14>( a, b );
            this->operator()( b, comp );
        }
     
        // This should be a depth-optimal solution.
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[ 2],a[13],comp);
            CompSwap(a[ 3],a[12],comp);
            CompSwap(a[ 4],a[11],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            
            CompSwap(a[ 0],a[ 7],comp);
            CompSwap(a[ 1],a[ 6],comp);
            CompSwap(a[ 2],a[ 5],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[10],a[13],comp);
            CompSwap(a[11],a[12],comp);
            
            CompSwap(a[ 0],a[ 3],comp);
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 4],a[ 7],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 8],a[11],comp);
            CompSwap(a[ 9],a[10],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 2],a[ 8],comp);
            CompSwap(a[10],a[11],comp);
            CompSwap(a[ 3],a[ 9],comp);
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[12],comp);
            CompSwap(a[ 7],a[13],comp);
            
            CompSwap(a[ 1],a[ 4],comp);
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[12],a[13],comp);
            
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[11],a[12],comp);
            CompSwap(a[ 6],a[10],comp);
            CompSwap(a[ 7],a[ 9],comp);
            
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 4],a[ 6],comp);
            CompSwap(a[ 9],a[11],comp);
            CompSwap(a[12],a[13],comp);
            CompSwap(a[ 5],a[ 7],comp);
            CompSwap(a[ 8],a[10],comp);
            
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[ 8],a[ 9],comp);
            CompSwap(a[10],a[11],comp);
            
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[ 9],a[10],comp);
            CompSwap(a[11],a[12],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<15,T>
    
    {   template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<15>( a, b );
            this->operator()( b, comp );
        }
     
        // This should be a depth-optimal solution.
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[ 1],a[14],comp);
            CompSwap(a[ 2],a[13],comp);
            CompSwap(a[ 3],a[12],comp);
            CompSwap(a[ 4],a[11],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            
            CompSwap(a[ 0],a[ 7],comp);
            CompSwap(a[ 1],a[ 6],comp);
            CompSwap(a[ 2],a[ 5],comp);
            CompSwap(a[ 3],a[ 4],comp);
            
            CompSwap(a[ 9],a[14],comp);
            CompSwap(a[10],a[13],comp);
            CompSwap(a[11],a[12],comp);
            
            CompSwap(a[ 0],a[ 3],comp);
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 4],a[ 7],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 8],a[11],comp);
            CompSwap(a[ 9],a[10],comp);
            CompSwap(a[13],a[14],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 2],a[ 8],comp);
            CompSwap(a[10],a[11],comp);
            CompSwap(a[ 3],a[ 9],comp);
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[12],comp);
            CompSwap(a[ 7],a[13],comp);
            
            
            CompSwap(a[ 1],a[ 4],comp);
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[11],a[14],comp);
            CompSwap(a[12],a[13],comp);
            
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[11],a[12],comp);
            CompSwap(a[13],a[14],comp);
            CompSwap(a[ 6],a[10],comp);
            CompSwap(a[ 7],a[ 9],comp);
            
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 4],a[ 6],comp);
            CompSwap(a[ 9],a[11],comp);
            CompSwap(a[12],a[13],comp);
            CompSwap(a[ 5],a[ 7],comp);
            CompSwap(a[ 8],a[10],comp);
            
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[ 8],a[ 9],comp);
            CompSwap(a[10],a[11],comp);
            
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[ 9],a[10],comp);
            CompSwap(a[11],a[12],comp);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<16,T>
    {
        template<typename C = std::less<T>>
        void operator()( cptr<T> a, mptr<T> b,C comp = C() )
        {
            copy_buffer<16>( a, b );
            this->operator()( b, comp );
        }
        
        // This should be a depth-optimal solution.
        template<typename C = std::less<T>>
        void operator()( mptr<T> a, C comp = C() )
        {
            CompSwap(a[ 0],a[15],comp);
            CompSwap(a[ 1],a[14],comp);
            CompSwap(a[ 2],a[13],comp);
            CompSwap(a[ 3],a[12],comp);
            CompSwap(a[ 4],a[11],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            
            CompSwap(a[ 0],a[ 7],comp);
            CompSwap(a[ 1],a[ 6],comp);
            CompSwap(a[ 2],a[ 5],comp);
            CompSwap(a[ 3],a[ 4],comp);
            
            CompSwap(a[ 8],a[15],comp);
            CompSwap(a[ 9],a[14],comp);
            CompSwap(a[10],a[13],comp);
            CompSwap(a[11],a[12],comp);
            
            CompSwap(a[ 0],a[ 3],comp);
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 4],a[ 7],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 8],a[11],comp);
            CompSwap(a[ 9],a[10],comp);
            CompSwap(a[12],a[15],comp);
            CompSwap(a[13],a[14],comp);
            
            CompSwap(a[ 0],a[ 1],comp);
            CompSwap(a[ 2],a[ 8],comp);
            CompSwap(a[10],a[11],comp);
            CompSwap(a[14],a[15],comp);
            CompSwap(a[ 3],a[ 9],comp);
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[12],comp);
            CompSwap(a[ 7],a[13],comp);
            
            
            CompSwap(a[ 1],a[ 4],comp);
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 5],a[10],comp);
            CompSwap(a[ 6],a[ 9],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[11],a[14],comp);
            CompSwap(a[12],a[13],comp);
            
            CompSwap(a[ 1],a[ 2],comp);
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 8],comp);
            CompSwap(a[11],a[12],comp);
            CompSwap(a[13],a[14],comp);
            CompSwap(a[ 6],a[10],comp);
            CompSwap(a[ 7],a[ 9],comp);
            
            CompSwap(a[ 2],a[ 3],comp);
            CompSwap(a[ 4],a[ 6],comp);
            CompSwap(a[ 9],a[11],comp);
            CompSwap(a[12],a[13],comp);
            CompSwap(a[ 5],a[ 7],comp);
            CompSwap(a[ 8],a[10],comp);
            
            CompSwap(a[ 4],a[ 5],comp);
            CompSwap(a[ 6],a[ 7],comp);
            CompSwap(a[ 8],a[ 9],comp);
            CompSwap(a[10],a[11],comp);
            
            CompSwap(a[ 3],a[ 4],comp);
            CompSwap(a[ 5],a[ 6],comp);
            CompSwap(a[ 7],a[ 8],comp);
            CompSwap(a[ 9],a[10],comp);
            CompSwap(a[11],a[12],comp);
        }
    };
    
    template<typename T, typename C = std::less<T>>
    void Sort( mptr<T> begin, mptr<T> end, C comp = C() )
    {
        // Use sorting networks for inputs of length <= 16; use std::sort otherwise.
        switch( std::distance( begin, end ) )
        {
            case 2:
            {
                SortNet<2,T>()(begin,comp);
                break;
            }
            case 3:
            {
                SortNet<3,T>()(begin,comp);
                break;
            }
            case 4:
            {
                SortNet<4,T>()(begin,comp);
                break;
            }
            case 5:
            {
                SortNet<5,T>()(begin,comp);
                break;
            }
            case 6:
            {
                SortNet<6,T>()(begin,comp);
                break;
            }
            case 7:
            {
                SortNet<7,T>()(begin,comp);
                break;
            }
            case 8:
            {
                SortNet<8,T>()(begin,comp);
                break;
            }
            case 9:
            {
                SortNet<9,T>()(begin,comp);
                break;
            }
            case 10:
            {
                SortNet<10,T>()(begin,comp);
                break;
            }
            case 11:
            {
                SortNet<11,T>()(begin,comp);
                break;
            }
            case 12:
            {
                SortNet<12,T>()(begin,comp);
                break;
            }
            case 13:
            {
                SortNet<13,T>()(begin,comp);
                break;
            }
            case 14:
            {
                SortNet<14,T>()(begin,comp);
                break;
            }
            case 15:
            {
                SortNet<15,T>()(begin,comp);
                break;
            }
            case 16:
            {
                SortNet<16,T>()(begin,comp);
                break;
            }
            case 1:
            {
                break;
            }
            default:
            {
                std::sort( begin, end, comp );
                break;
            }
        }
    }
    

} // namespace Tools
