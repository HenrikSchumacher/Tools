#pragma once

namespace Tools
{
    template<int n, typename T>
    struct SortNet
    {
        void operator()( mut<T> a )
        {
            std::sort( a, a+n );
        }
    };
    
    template<typename T>
    force_inline void CompSwap( T & a, T & b)
    {
        const std::pair<T,T> p = std::minmax(a,b);

        a = p.first;
        b = p.second;
    }
    

    
    template<typename T>
    struct SortNet<1,T>
    {
        void operator()( mut<T> a )
        {}
    };
    
    template<typename T>
    struct SortNet<2,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[1]);
        }
    };
    
    template<typename T>
    struct SortNet<3,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[1]);
            CompSwap(a[0],a[2]);
            CompSwap(a[1],a[2]);
        }
    };
    
    template<typename T>
    struct SortNet<4,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            
            CompSwap(a[0],a[2]);
            CompSwap(a[1],a[3]);
            
            CompSwap(a[1],a[2]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<5,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            
            CompSwap(a[1],a[3]);
            CompSwap(a[2],a[4]);
            
            CompSwap(a[0],a[2]);
            CompSwap(a[1],a[4]);
            
            CompSwap(a[1],a[2]);
            CompSwap(a[3],a[4]);
            
            CompSwap(a[2],a[3]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<6,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
            
            CompSwap(a[0],a[2]);
            CompSwap(a[3],a[5]);
            CompSwap(a[1],a[4]);
            
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
            
            CompSwap(a[1],a[2]);
            CompSwap(a[3],a[4]);

            CompSwap(a[2],a[3]);
        }
    };
    
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<7,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[1],a[2]);
            CompSwap(a[3],a[4]);
            CompSwap(a[5],a[6]);

            CompSwap(a[0],a[2]);
            CompSwap(a[4],a[6]);
            CompSwap(a[3],a[5]);

            CompSwap(a[2],a[6]);
            CompSwap(a[1],a[5]);
            CompSwap(a[0],a[4]);

            CompSwap(a[2],a[5]);
            CompSwap(a[0],a[3]);

            CompSwap(a[2],a[4]);
            CompSwap(a[1],a[3]);

            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<8,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[2]);
            CompSwap(a[1],a[3]);
            CompSwap(a[4],a[6]);
            CompSwap(a[5],a[7]);
            
            CompSwap(a[0],a[4]);
            CompSwap(a[1],a[5]);
            CompSwap(a[2],a[6]);
            CompSwap(a[3],a[7]);
            
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
            CompSwap(a[6],a[7]);
            
            CompSwap(a[2],a[4]);
            CompSwap(a[3],a[5]);
            
            CompSwap(a[1],a[4]);
            CompSwap(a[3],a[6]);
            
            CompSwap(a[1],a[2]);
            CompSwap(a[3],a[4]);
            CompSwap(a[5],a[6]);
            
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<9,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[1],a[8]);
            CompSwap(a[2],a[7]);
            CompSwap(a[3],a[6]);
            CompSwap(a[4],a[5]);
            
            CompSwap(a[1],a[4]);
            CompSwap(a[0],a[2]);
            CompSwap(a[5],a[8]);
            CompSwap(a[6],a[7]);
            
            CompSwap(a[2],a[6]);
            CompSwap(a[7],a[8]);
            CompSwap(a[0],a[3]);
            CompSwap(a[4],a[5]);
            
            CompSwap(a[0],a[1]);
            CompSwap(a[3],a[5]);
            CompSwap(a[6],a[7]);
            CompSwap(a[2],a[4]);
            
            CompSwap(a[1],a[3]);
            CompSwap(a[5],a[7]);
            CompSwap(a[4],a[6]);
            
            CompSwap(a[1],a[2]);
            CompSwap(a[3],a[4]);
            CompSwap(a[5],a[6]);
            CompSwap(a[7],a[8]);
            
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<10,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
            CompSwap(a[6],a[7]);
            CompSwap(a[8],a[9]);
            
            CompSwap(a[4],a[9]);
            CompSwap(a[0],a[5]);
            CompSwap(a[1],a[8]);
            CompSwap(a[3],a[7]);
            CompSwap(a[2],a[6]);
            
            CompSwap(a[0],a[2]);
            CompSwap(a[3],a[6]);
            CompSwap(a[7],a[9]);
            CompSwap(a[1],a[4]);
            CompSwap(a[5],a[8]);
            
            CompSwap(a[0],a[1]);
            CompSwap(a[2],a[7]);
            CompSwap(a[8],a[9]);
            CompSwap(a[4],a[6]);
            CompSwap(a[3],a[5]);
            
            CompSwap(a[2],a[4]);
            CompSwap(a[6],a[8]);
            CompSwap(a[1],a[3]);
            CompSwap(a[5],a[7]);
            
            CompSwap(a[1],a[2]);
            CompSwap(a[3],a[4]);
            CompSwap(a[5],a[6]);
            CompSwap(a[7],a[8]);
            
            CompSwap(a[2],a[3]);
            CompSwap(a[4],a[5]);
            CompSwap(a[6],a[7]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<11,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[ 0],a[ 9]);
            CompSwap(a[ 1],a[ 8]);
            CompSwap(a[ 2],a[ 7]);
            CompSwap(a[ 3],a[ 6]);
            CompSwap(a[ 4],a[ 5]);
            
            CompSwap(a[ 0],a[ 3]);
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 4],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[ 9],a[10]);
            
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 4],a[ 6]);
            CompSwap(a[ 8],a[10]);
            CompSwap(a[ 5],a[ 9]);
            
            CompSwap(a[ 0],a[ 4]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[ 1],a[ 5]);
            CompSwap(a[ 2],a[ 9]);
            CompSwap(a[ 3],a[ 6]);
            
            CompSwap(a[ 1],a[ 4]);
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 5],a[ 7]);
            CompSwap(a[ 6],a[ 9]);
            
            CompSwap(a[ 2],a[ 4]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[ 8],a[ 9]);
            CompSwap(a[ 3],a[ 5]);
            
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 7],a[ 8]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<12,T>
    {
        void operator()( mut<T> a )
        {
            CompSwap(a[ 0],a[ 6]);
            CompSwap(a[ 1],a[ 7]);
            CompSwap(a[ 2],a[ 8]);
            CompSwap(a[ 3],a[ 9]);
            CompSwap(a[ 4],a[10]);
            CompSwap(a[ 5],a[11]);
            
            CompSwap(a[ 0],a[ 3]);
            CompSwap(a[ 1],a[ 4]);
            CompSwap(a[ 2],a[ 5]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[10]);
            CompSwap(a[ 8],a[11]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[10],a[11]);
            
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 3],a[ 6]);
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[ 9],a[10]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 2],a[ 9]);
            CompSwap(a[10],a[11]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[ 6],a[ 7]);
            
            CompSwap(a[ 1],a[ 3]);
            CompSwap(a[ 4],a[ 7]);
            CompSwap(a[ 8],a[10]);
            CompSwap(a[ 2],a[ 6]);
            CompSwap(a[ 5],a[ 9]);
            
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 4],a[ 6]);
            CompSwap(a[ 8],a[ 9]);
            CompSwap(a[ 5],a[ 7]);
            
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 7],a[ 8]);
        }
    };
   
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<13,T>
    {
        // This should be a depth-optimal solution.
        void operator()( mut<T> a )
        {
            CompSwap(a[ 3],a[12]);
            CompSwap(a[ 4],a[11]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            
            CompSwap(a[ 0],a[ 7]);
            CompSwap(a[ 1],a[ 6]);
            CompSwap(a[ 2],a[ 5]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[11],a[12]);
            
            CompSwap(a[ 0],a[ 3]);
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 4],a[ 7]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 8],a[11]);
            CompSwap(a[ 9],a[10]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 2],a[ 8]);
            CompSwap(a[10],a[11]);
            CompSwap(a[ 3],a[ 9]);
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[12]);
            
            CompSwap(a[ 1],a[ 4]);
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[11],a[12]);
            CompSwap(a[ 6],a[10]);
            CompSwap(a[ 7],a[ 9]);
            
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 4],a[ 6]);
            CompSwap(a[ 9],a[11]);
            CompSwap(a[ 5],a[ 7]);
            CompSwap(a[ 8],a[10]);
            
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[ 8],a[ 9]);
            CompSwap(a[10],a[11]);
            
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[ 9],a[10]);
            CompSwap(a[11],a[12]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<14,T>
    {
        // This should be a depth-optimal solution.
        void operator()( mut<T> a )
        {
            CompSwap(a[ 2],a[13]);
            CompSwap(a[ 3],a[12]);
            CompSwap(a[ 4],a[11]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            
            CompSwap(a[ 0],a[ 7]);
            CompSwap(a[ 1],a[ 6]);
            CompSwap(a[ 2],a[ 5]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[10],a[13]);
            CompSwap(a[11],a[12]);
            
            CompSwap(a[ 0],a[ 3]);
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 4],a[ 7]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 8],a[11]);
            CompSwap(a[ 9],a[10]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 2],a[ 8]);
            CompSwap(a[10],a[11]);
            CompSwap(a[ 3],a[ 9]);
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[12]);
            CompSwap(a[ 7],a[13]);
            
            CompSwap(a[ 1],a[ 4]);
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[12],a[13]);
            
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[11],a[12]);
            CompSwap(a[ 6],a[10]);
            CompSwap(a[ 7],a[ 9]);
            
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 4],a[ 6]);
            CompSwap(a[ 9],a[11]);
            CompSwap(a[12],a[13]);
            CompSwap(a[ 5],a[ 7]);
            CompSwap(a[ 8],a[10]);
            
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[ 8],a[ 9]);
            CompSwap(a[10],a[11]);
            
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[ 9],a[10]);
            CompSwap(a[11],a[12]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<15,T>
    {
        // This should be a depth-optimal solution.
        void operator()( mut<T> a )
        {
            CompSwap(a[ 1],a[14]);
            CompSwap(a[ 2],a[13]);
            CompSwap(a[ 3],a[12]);
            CompSwap(a[ 4],a[11]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            
            CompSwap(a[ 0],a[ 7]);
            CompSwap(a[ 1],a[ 6]);
            CompSwap(a[ 2],a[ 5]);
            CompSwap(a[ 3],a[ 4]);
            
            CompSwap(a[ 9],a[14]);
            CompSwap(a[10],a[13]);
            CompSwap(a[11],a[12]);
            
            CompSwap(a[ 0],a[ 3]);
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 4],a[ 7]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 8],a[11]);
            CompSwap(a[ 9],a[10]);
            CompSwap(a[13],a[14]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 2],a[ 8]);
            CompSwap(a[10],a[11]);
            CompSwap(a[ 3],a[ 9]);
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[12]);
            CompSwap(a[ 7],a[13]);
            
            
            CompSwap(a[ 1],a[ 4]);
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[11],a[14]);
            CompSwap(a[12],a[13]);
            
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[11],a[12]);
            CompSwap(a[13],a[14]);
            CompSwap(a[ 6],a[10]);
            CompSwap(a[ 7],a[ 9]);
            
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 4],a[ 6]);
            CompSwap(a[ 9],a[11]);
            CompSwap(a[12],a[13]);
            CompSwap(a[ 5],a[ 7]);
            CompSwap(a[ 8],a[10]);
            
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[ 8],a[ 9]);
            CompSwap(a[10],a[11]);
            
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[ 9],a[10]);
            CompSwap(a[11],a[12]);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<typename T>
    struct SortNet<16,T>
    {
        // This should be a depth-optimal solution.
        void operator()( mut<T> a )
        {
            CompSwap(a[ 0],a[15]);
            CompSwap(a[ 1],a[14]);
            CompSwap(a[ 2],a[13]);
            CompSwap(a[ 3],a[12]);
            CompSwap(a[ 4],a[11]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            
            CompSwap(a[ 0],a[ 7]);
            CompSwap(a[ 1],a[ 6]);
            CompSwap(a[ 2],a[ 5]);
            CompSwap(a[ 3],a[ 4]);
            
            CompSwap(a[ 8],a[15]);
            CompSwap(a[ 9],a[14]);
            CompSwap(a[10],a[13]);
            CompSwap(a[11],a[12]);
            
            CompSwap(a[ 0],a[ 3]);
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 4],a[ 7]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 8],a[11]);
            CompSwap(a[ 9],a[10]);
            CompSwap(a[12],a[15]);
            CompSwap(a[13],a[14]);
            
            CompSwap(a[ 0],a[ 1]);
            CompSwap(a[ 2],a[ 8]);
            CompSwap(a[10],a[11]);
            CompSwap(a[14],a[15]);
            CompSwap(a[ 3],a[ 9]);
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[12]);
            CompSwap(a[ 7],a[13]);
            
            
            CompSwap(a[ 1],a[ 4]);
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 5],a[10]);
            CompSwap(a[ 6],a[ 9]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[11],a[14]);
            CompSwap(a[12],a[13]);
            
            CompSwap(a[ 1],a[ 2]);
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 8]);
            CompSwap(a[11],a[12]);
            CompSwap(a[13],a[14]);
            CompSwap(a[ 6],a[10]);
            CompSwap(a[ 7],a[ 9]);
            
            CompSwap(a[ 2],a[ 3]);
            CompSwap(a[ 4],a[ 6]);
            CompSwap(a[ 9],a[11]);
            CompSwap(a[12],a[13]);
            CompSwap(a[ 5],a[ 7]);
            CompSwap(a[ 8],a[10]);
            
            CompSwap(a[ 4],a[ 5]);
            CompSwap(a[ 6],a[ 7]);
            CompSwap(a[ 8],a[ 9]);
            CompSwap(a[10],a[11]);
            
            CompSwap(a[ 3],a[ 4]);
            CompSwap(a[ 5],a[ 6]);
            CompSwap(a[ 7],a[ 8]);
            CompSwap(a[ 9],a[10]);
            CompSwap(a[11],a[12]);
        }
    };
    
    template<typename T>
    void Sort( mut<T> begin, mut<T> end )
    {
        // Use sorting networks for inputs of length <= 16; use std::sort otherwise.
        switch( std::distance( begin, end ) )
        {
            case 2:
            {
                SortNet<2,T>()( begin );
                break;
            }
            case 3:
            {
                SortNet<3,T>()( begin );
                break;
            }
            case 4:
            {
                SortNet<4,T>()( begin );
                break;
            }
            case 5:
            {
                SortNet<5,T>()( begin );
                break;
            }
            case 6:
            {
                SortNet<6,T>()( begin );
                break;
            }
            case 7:
            {
                SortNet<7,T>()( begin );
                break;
            }
            case 8:
            {
                SortNet<8,T>()( begin );
                break;
            }
            case 9:
            {
                SortNet<9,T>()( begin );
                break;
            }
            case 10:
            {
                SortNet<10,T>()( begin );
                break;
            }
            case 11:
            {
                SortNet<11,T>()( begin );
                break;
            }
            case 12:
            {
                SortNet<12,T>()( begin );
                break;
            }
            case 13:
            {
                SortNet<13,T>()( begin );
                break;
            }
            case 14:
            {
                SortNet<14,T>()( begin );
                break;
            }
            case 15:
            {
                SortNet<15,T>()( begin );
                break;
            }
            case 16:
            {
                SortNet<16,T>()( begin );
                break;
            }
            case 1:
            {
                break;
            }
            default:
            {
                std::sort( begin, end );
                break;
            }
        }
    }
}
