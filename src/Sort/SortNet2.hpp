#pragma once

#include <functional>

namespace Tools
{
    template<Size_T n, bool reverseQ = false>
    struct SortNet2
    {
//        template<typename T, typename CompSwap_T>
//        void operator()( CompSwap_T s ) = 0;
//        
//        {
//            std::sort( a, a+n, comp );
//        }
    };

    template<> struct SortNet2<1>
    {
        template<typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            (void)s;
        }
    };
    
    template<> struct SortNet2<2>
    {
        template<typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
        }
    };

    template<> struct SortNet2<3>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
            s( 0,2);
            s( 1,2);
        }
    };

    template<> struct SortNet2<4>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
            s( 2,3);
            
            s( 0,2);
            s( 1,3);
            
            s( 1,2);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<5>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
            s( 2,3);
            
            s( 1,3);
            s( 2,4);
            
            s( 0,2);
            s( 1,4);
            
            s( 1,2);
            s( 3,4);
            
            s( 2,3);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<6>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
            s( 2,3);
            s( 4,5);
            
            s( 0,2);
            s( 3,5);
            s( 1,4);
            
            s( 0, 1);
            s( 2,3);
            s( 4,5);
            
            s( 1,2);
            s( 3,4);

            s( 2,3);
        }
    };
    
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<7>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 1,2);
            s( 3,4);
            s( 5,6);

            s( 0,2);
            s( 4,6);
            s( 3,5);

            s( 2,6);
            s( 1,5);
            s( 0,4);

            s( 2,5);
            s( 0,3);

            s( 2,4);
            s( 1,3);

            s( 0, 1);
            s( 2,3);
            s( 4,5);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<8>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0,2);
            s( 1,3);
            s( 4,6);
            s( 5,7);
            
            s( 0,4);
            s( 1,5);
            s( 2,6);
            s( 3,7);
            
            s( 0, 1);
            s( 2,3);
            s( 4,5);
            s( 6,7);
            
            s( 2,4);
            s( 3,5);
            
            s( 1,4);
            s( 3,6);
            
            s( 1,2);
            s( 3,4);
            s( 5,6);
            
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<9>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 1, 8);
            s( 2,7);
            s( 3,6);
            s( 4,5);
            
            s( 1,4);
            s( 0,2);
            s( 5, 8);
            s( 6,7);
            
            s( 2,6);
            s( 7, 8);
            s( 0,3);
            s( 4,5);
            
            s( 0, 1);
            s( 3,5);
            s( 6,7);
            s( 2,4);
            
            s( 1,3);
            s( 5,7);
            s( 4,6);
            
            s( 1,2);
            s( 3,4);
            s( 5,6);
            s( 7, 8);
            
            s( 2,3);
            s( 4,5);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<10>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
            s( 2,3);
            s( 4,5);
            s( 6,7);
            s( 8,9);
            
            s( 4,9);
            s( 0,5);
            s( 1, 8);
            s( 3,7);
            s( 2,6);
            
            s( 0,2);
            s( 3,6);
            s( 7,9);
            s( 1,4);
            s( 5, 8);
            
            s( 0, 1);
            s( 2,7);
            s( 8,9);
            s( 4,6);
            s( 3,5);
            
            s( 2,4);
            s( 6, 8);
            s( 1,3);
            s( 5,7);
            
            s( 1,2);
            s( 3,4);
            s( 5,6);
            s( 7, 8);
            
            s( 2,3);
            s( 4,5);
            s( 6,7);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<11>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 9);
            s( 1, 8);
            s( 2, 7);
            s( 3, 6);
            s( 4, 5);
            
            s( 0, 3);
            s( 1, 2);
            s( 4,10);
            s( 6, 9);
            s( 7, 8);
            
            s( 0, 1);
            s( 2, 3);
            s( 5, 8);
            s( 6, 7);
            s( 9,10);
            
            s( 1, 2);
            s( 4, 6);
            s( 8,10);
            s( 5, 9);
            
            s( 0, 4);
            s( 7, 8);
            s( 1, 5);
            s( 2, 9);
            s( 3, 6);
            
            s( 1, 4);
            s( 2, 3);
            s( 5, 7);
            s( 6, 9);
            
            s( 2, 4);
            s( 6, 7);
            s( 8, 9);
            s( 3, 5);
            
            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<12>
    {
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 6);
            s( 1, 7);
            s( 2, 8);
            s( 3, 9);
            s( 4,10);
            s( 5,11);
            
            s( 0, 3);
            s( 1, 4);
            s( 2, 5);
            s( 6, 9);
            s( 7,10);
            s( 8,11);
            
            s( 0, 1);
            s( 3, 4);
            s( 5, 8);
            s( 6, 7);
            s(10,11);
            
            s( 1, 2);
            s( 3, 6);
            s( 4, 5);
            s( 7, 8);
            s( 9,10);
            
            s( 0, 1);
            s( 2, 9);
            s(10,11);
            s( 3, 4);
            s( 5, 8);
            s( 6, 7);
            
            s( 1, 3);
            s( 4, 7);
            s( 8,10);
            s( 2, 6);
            s( 5, 9);
            
            s( 2, 3);
            s( 4, 6);
            s( 8, 9);
            s( 5, 7);
            
            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
        }
    };
   
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<13>
    {
        // This should be a depth-optimal solution.
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 3,12);
            s( 4,11);
            s( 5,10);
            s( 6, 9);
            s( 7, 8);
            
            s( 0, 7);
            s( 1, 6);
            s( 2, 5);
            s( 3, 4);
            s(11,12);
            
            s( 0, 3);
            s( 1, 2);
            s( 4, 7);
            s( 5, 6);
            s( 8,11);
            s( 9,10);
            
            s( 0, 1);
            s( 2, 8);
            s(10,11);
            s( 3, 9);
            s( 4, 5);
            s( 6,12);
            
            s( 1, 4);
            s( 2, 3);
            s( 5,10);
            s( 6, 9);
            s( 7, 8);
            
            s( 1, 2);
            s( 3, 4);
            s( 5, 8);
            s(11,12);
            s( 6,10);
            s( 7, 9);
            
            s( 2, 3);
            s( 4, 6);
            s( 9,11);
            s( 5, 7);
            s( 8,10);
            
            s( 4, 5);
            s( 6, 7);
            s( 8, 9);
            s(10,11);
            
            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
            s( 9,10);
            s(11,12);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<14>
    {
        // This should be a depth-optimal solution.
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 2,13);
            s( 3,12);
            s( 4,11);
            s( 5,10);
            s( 6, 9);
            s( 7, 8);
            
            s( 0, 7);
            s( 1, 6);
            s( 2, 5);
            s( 3, 4);
            s(10,13);
            s(11,12);
            
            s( 0, 3);
            s( 1, 2);
            s( 4, 7);
            s( 5, 6);
            s( 8,11);
            s( 9,10);
            
            s( 0, 1);
            s( 2, 8);
            s(10,11);
            s( 3, 9);
            s( 4, 5);
            s( 6,12);
            s( 7,13);
            
            s( 1, 4);
            s( 2, 3);
            s( 5,10);
            s( 6, 9);
            s( 7, 8);
            s(12,13);
            
            s( 1, 2);
            s( 3, 4);
            s( 5, 8);
            s(11,12);
            s( 6,10);
            s( 7, 9);
            
            s( 2, 3);
            s( 4, 6);
            s( 9,11);
            s(12,13);
            s( 5, 7);
            s( 8,10);
            
            s( 4, 5);
            s( 6, 7);
            s( 8, 9);
            s(10,11);
            
            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
            s( 9,10);
            s(11,12);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<15>
    {
        // This should be a depth-optimal solution.
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 1,14);
            s( 2,13);
            s( 3,12);
            s( 4,11);
            s( 5,10);
            s( 6, 9);
            s( 7, 8);
            
            s( 0, 7);
            s( 1, 6);
            s( 2, 5);
            s( 3, 4);
            
            s( 9,14);
            s(10,13);
            s(11,12);
            
            s( 0, 3);
            s( 1, 2);
            s( 4, 7);
            s( 5, 6);
            s( 8,11);
            s( 9,10);
            s(13,14);
            
            s( 0, 1);
            s( 2, 8);
            s(10,11);
            s( 3, 9);
            s( 4, 5);
            s( 6,12);
            s( 7,13);
            
            
            s( 1, 4);
            s( 2, 3);
            s( 5,10);
            s( 6, 9);
            s( 7, 8);
            s(11,14);
            s(12,13);
            
            s( 1, 2);
            s( 3, 4);
            s( 5, 8);
            s(11,12);
            s(13,14);
            s( 6,10);
            s( 7, 9);
            
            s( 2, 3);
            s( 4, 6);
            s( 9,11);
            s(12,13);
            s( 5, 7);
            s( 8,10);
            
            s( 4, 5);
            s( 6, 7);
            s( 8, 9);
            s(10,11);
            
            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
            s( 9,10);
            s(11,12);
        }
    };
    
    // https://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
    template<> struct SortNet2<16>
    {
        // This should be a depth-optimal solution.
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            // https://bertdobbelaere.github.io/sorting_networks.html#N32L185D14
            
            s( 0,13);
            s( 1,12);
            s( 2,15);
            s( 3,14);
            s( 4, 8);
            s( 5, 6);
            s( 7,11);
            s( 9,10);

            s( 0, 5);
            s( 1, 7);
            s( 2, 9);
            s( 3, 4);
            s( 6,13);
            s( 8,14);
            s(10,15);
            s(11,12);

            s( 0, 1);
            s( 2, 3);
            s( 4, 5);
            s( 6, 8);
            s( 7, 9);
            s(10,11);
            s(12,13);
            s(14,15);

            s( 0, 2);
            s( 1, 3);
            s( 4,10);
            s( 5,11);
            s( 6, 7);
            s( 8, 9);
            s(12,14);
            s(13,15);

            s( 1, 2);
            s( 3,12);
            s( 4, 6);
            s( 5, 7);
            s( 8,10);
            s( 9,11);
            s(13,14);

            s( 1, 4);
            s( 2, 6);
            s( 5, 8);
            s( 7,10);
            s( 9,13);
            s(11,14);

            s( 2, 4);
            s( 3, 6);
            s( 9,12);
            s(11,13);

            s( 3, 5);
            s( 6, 8);
            s( 7, 9);
            s(10,12);

            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
            s( 9,10);
            s(11,12);

            s( 6, 7);
            s( 8, 9);
            
            
            
//            s( 0,15);
//            s( 1,14);
//            s( 2,13);
//            s( 3,12);
//            s( 4,11);
//            s( 5,10);
//            s( 6, 9);
//            s( 7, 8);
//
//            s( 0, 7);
//            s( 1, 6);
//            s( 2, 5);
//            s( 3, 4);
//
//            s( 8,15);
//            s( 9,14);
//            s(10,13);
//            s(11,12);
//
//            s( 0, 3);
//            s( 1, 2);
//            s( 4, 7);
//            s( 5, 6);
//            s( 8,11);
//            s( 9,10);
//            s(12,15);
//            s(13,14);
//
//            s( 0, 1);
//            s( 2, 8);
//            s(10,11);
//            s(14,15);
//            s( 3, 9);
//            s( 4, 5);
//            s( 6,12);
//            s( 7,13);
//
//
//            s( 1, 4);
//            s( 2, 3);
//            s( 5,10);
//            s( 6, 9);
//            s( 7, 8);
//            s(11,14);
//            s(12,13);
//
//            s( 1, 2);
//            s( 3, 4);
//            s( 5, 8);
//            s(11,12);
//            s(13,14);
//            s( 6,10);
//            s( 7, 9);
//
//            s( 2, 3);
//            s( 4, 6);
//            s( 9,11);
//            s(12,13);
//            s( 5, 7);
//            s( 8,10);
//
//            s( 4, 5);
//            s( 6, 7);
//            s( 8, 9);
//            s(10,11);
//
//            s( 3, 4);
//            s( 5, 6);
//            s( 7, 8);
//            s( 9,10);
//            s(11,12);
        }
    };
    
    // https://bertdobbelaere.github.io/sorting_networks.html#N32L185D14
    template<> struct SortNet2<32>
    {
        // This should be a depth-optimal solution.
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 1);
            s( 2, 3);
            s( 4, 5);
            s( 6, 7);
            s( 8, 9);
            s(10,11);
            s(12,13);
            s(14,15);
            s(16,17);
            s(18,19);
            s(20,21);
            s(22,23);
            s(24,25);
            s(26,27);
            s(28,29);
            s(30,31);

            s( 0, 2);
            s( 1, 3);
            s( 4, 6);
            s( 5, 7);
            s( 8,10);
            s( 9,11);
            s(12,14);
            s(13,15);
            s(16,18);
            s(17,19);
            s(20,22);
            s(21,23);
            s(24,26);
            s(25,27);
            s(28,30);
            s(29,31);

            s( 0, 4);
            s( 1, 5);
            s( 2, 6);
            s( 3, 7);
            s( 8,12);
            s( 9,13);
            s(10,14);
            s(11,15);
            s(16,20);
            s(17,21);
            s(18,22);
            s(19,23);
            s(24,28);
            s(25,29);
            s(26,30);
            s(27,31);

            s( 0, 8);
            s( 1, 9);
            s( 2,10);
            s( 3,11);
            s( 4,12);
            s( 5,13);
            s( 6,14);
            s( 7,15);
            s(16,24);
            s(17,25);
            s(18,26);
            s(19,27);
            s(20,28);
            s(21,29);
            s(22,30);
            s(23,31);

            s( 0,16);
            s( 1, 8);
            s( 2, 4);
            s( 3,12);
            s( 5,10);
            s( 6, 9);
            s( 7,14);
            s(11,13);
            s(15,31);
            s(17,24);
            s(18,20);
            s(19,28);
            s(21,26);
            s(22,25);
            s(23,30);
            s(27,29);

            s( 1, 2);
            s( 3, 5);
            s( 4, 8);
            s( 6,22);
            s( 7,11);
            s( 9,25);
            s(10,12);
            s(13,14);
            s(17,18);
            s(19,21);
            s(20,24);
            s(23,27);
            s(26,28);
            s(29,30);

            s( 1,17);
            s( 2,18);
            s( 3,19);
            s( 4,20);
            s( 5,10);
            s( 7,23);
            s( 8,24);
            s(11,27);
            s(12,28);
            s(13,29);
            s(14,30);
            s(21,26);

            s( 3,17);
            s( 4,16);
            s( 5,21);
            s( 6,18);
            s( 7, 9);
            s( 8,20);
            s(10,26);
            s(11,23);
            s(13,25);
            s(14,28);
            s(15,27);
            s(22,24);

            s( 1, 4);
            s( 3, 8);
            s( 5,16);
            s( 7,17);
            s( 9,21);
            s(10,22);
            s(11,19);
            s(12,20);
            s(14,24);
            s(15,26);
            s(23,28);
            s(27,30);

            s( 2, 5);
            s( 7, 8);
            s( 9,18);
            s(11,17);
            s(12,16);
            s(13,22);
            s(14,20);
            s(15,19);
            s(23,24);
            s(26,29);

            s( 2, 4);
            s( 6,12);
            s( 9,16);
            s(10,11);
            s(13,17);
            s(14,18);
            s(15,22);
            s(19,25);
            s(20,21);
            s(27,29);

            s( 5, 6);
            s( 8,12);
            s( 9,10);
            s(11,13);
            s(14,16);
            s(15,17);
            s(18,20);
            s(19,23);
            s(21,22);
            s(25,26);

            s( 3, 5);
            s( 6, 7);
            s( 8, 9);
            s(10,12);
            s(11,14);
            s(13,16);
            s(15,18);
            s(17,20);
            s(19,21);
            s(22,23);
            s(24,25);
            s(26,28);

            s( 3, 4);
            s( 5, 6);
            s( 7, 8);
            s( 9,10);
            s(11,12);
            s(13,14);
            s(15,16);
            s(17,18);
            s(19,20);
            s(21,22);
            s(23,24);
            s(25,26);
            s(27,28);
        }
    };
    
    // https://bertdobbelaere.github.io/sorting_networks_extended.html
    template<> struct SortNet2<64>
    {
        // This should be a depth-optimal solution.
        template<typename T, typename CompSwap_T>
        void operator()( CompSwap_T s )
        {
            s( 0, 2);
            s( 1, 3);
            s( 4, 6);
            s( 5, 7);
            s( 8,10);
            s( 9,11);
            s(12,14);
            s(13,15);
            s(16,18);
            s(17,19);
            s(20,22);
            s(21,23);
            s(24,26);
            s(25,27);
            s(28,30);
            s(29,31);
            s(32,34);
            s(33,35);
            s(36,38);
            s(37,39);
            s(40,42);
            s(41,43);
            s(44,46);
            s(45,47);
            s(48,50);
            s(49,51);
            s(52,54);
            s(53,55);
            s(56,58);
            s(57,59);
            s(60,62);
            s(61,63);

            s( 0, 1);
            s( 2, 3);
            s( 4, 5);
            s( 6, 7);
            s( 8, 9);
            s(10,11);
            s(12,13);
            s(14,15);
            s(16,17);
            s(18,19);
            s(20,21);
            s(22,23);
            s(24,25);
            s(26,27);
            s(28,29);
            s(30,31);
            s(32,33);
            s(34,35);
            s(36,37);
            s(38,39);
            s(40,41);
            s(42,43);
            s(44,45);
            s(46,47);
            s(48,49);
            s(50,51);
            s(52,53);
            s(54,55);
            s(56,57);
            s(58,59);
            s(60,61);
            s(62,63);

            s( 0,20);
            s( 1, 2);
            s( 3,23);
            s( 4,16);
            s( 5, 6);
            s( 7,19);
            s( 8,48);
            s( 9,10);
            s(11,51);
            s(12,52);
            s(13,14);
            s(15,55);
            s(17,18);
            s(21,22);
            s(24,28);
            s(25,26);
            s(27,31);
            s(29,30);
            s(32,36);
            s(33,34);
            s(35,39);
            s(37,38);
            s(40,60);
            s(41,42);
            s(43,63);
            s(44,56);
            s(45,46);
            s(47,59);
            s(49,50);
            s(53,54);
            s(57,58);
            s(61,62);

            s( 0, 8);
            s( 1,21);
            s( 2,22);
            s( 3,11);
            s( 4,40);
            s( 5,17);
            s( 6,18);
            s( 7,43);
            s( 9,49);
            s(10,50);
            s(12,24);
            s(13,53);
            s(14,54);
            s(15,27);
            s(16,28);
            s(19,31);
            s(20,56);
            s(23,59);
            s(25,29);
            s(26,30);
            s(32,44);
            s(33,37);
            s(34,38);
            s(35,47);
            s(36,48);
            s(39,51);
            s(41,61);
            s(42,62);
            s(45,57);
            s(46,58);
            s(52,60);
            s(55,63);

            s( 0,32);
            s( 1, 9);
            s( 2,10);
            s( 3,35);
            s( 4,12);
            s( 5,41);
            s( 6,42);
            s( 7,15);
            s( 8,44);
            s(11,47);
            s(13,25);
            s(14,26);
            s(16,52);
            s(17,29);
            s(18,30);
            s(19,55);
            s(20,36);
            s(21,57);
            s(22,58);
            s(23,39);
            s(24,40);
            s(27,43);
            s(28,60);
            s(31,63);
            s(33,45);
            s(34,46);
            s(37,49);
            s(38,50);
            s(48,56);
            s(51,59);
            s(53,61);
            s(54,62);

            s( 0, 4);
            s( 1,33);
            s( 2,34);
            s( 3, 7);
            s( 5,13);
            s( 6,14);
            s( 8,16);
            s( 9,45);
            s(10,46);
            s(11,19);
            s(12,32);
            s(15,35);
            s(17,53);
            s(18,54);
            s(20,24);
            s(21,37);
            s(22,38);
            s(23,27);
            s(25,41);
            s(26,42);
            s(28,48);
            s(29,61);
            s(30,62);
            s(31,51);
            s(36,40);
            s(39,43);
            s(44,52);
            s(47,55);
            s(49,57);
            s(50,58);
            s(56,60);
            s(59,63);

            s( 1, 5);
            s( 2, 6);
            s( 4,12);
            s( 7,15);
            s( 8,20);
            s( 9,17);
            s(10,18);
            s(11,23);
            s(13,33);
            s(14,34);
            s(16,32);
            s(19,35);
            s(21,25);
            s(22,26);
            s(24,36);
            s(27,39);
            s(28,44);
            s(29,49);
            s(30,50);
            s(31,47);
            s(37,41);
            s(38,42);
            s(40,52);
            s(43,55);
            s(45,53);
            s(46,54);
            s(48,56);
            s(51,59);
            s(57,61);
            s(58,62);

            s( 4, 8);
            s( 5,13);
            s( 6,14);
            s( 7,11);
            s( 9,21);
            s(10,22);
            s(12,20);
            s(15,23);
            s(16,44);
            s(17,33);
            s(18,34);
            s(19,47);
            s(24,32);
            s(25,37);
            s(26,38);
            s(27,35);
            s(28,36);
            s(29,45);
            s(30,46);
            s(31,39);
            s(40,48);
            s(41,53);
            s(42,54);
            s(43,51);
            s(49,57);
            s(50,58);
            s(52,56);
            s(55,59);

            s( 5, 9);
            s( 6,10);
            s( 8,12);
            s(11,15);
            s(13,21);
            s(14,22);
            s(16,20);
            s(17,45);
            s(18,46);
            s(19,23);
            s(24,28);
            s(25,33);
            s(26,34);
            s(27,31);
            s(29,37);
            s(30,38);
            s(32,36);
            s(35,39);
            s(40,44);
            s(41,49);
            s(42,50);
            s(43,47);
            s(48,52);
            s(51,55);
            s(53,57);
            s(54,58);

            s( 9,13);
            s(10,14);
            s(16,24);
            s(17,21);
            s(18,22);
            s(19,27);
            s(20,28);
            s(23,31);
            s(25,29);
            s(26,30);
            s(32,40);
            s(33,37);
            s(34,38);
            s(35,43);
            s(36,44);
            s(39,47);
            s(41,45);
            s(42,46);
            s(49,53);
            s(50,54);

            s(12,16);
            s(15,19);
            s(17,25);
            s(18,26);
            s(20,24);
            s(21,29);
            s(22,30);
            s(23,27);
            s(28,32);
            s(31,35);
            s(33,41);
            s(34,42);
            s(36,40);
            s(37,45);
            s(38,46);
            s(39,43);
            s(44,48);
            s(47,51);

            s( 1,16);
            s( 2,32);
            s( 5,20);
            s( 6,36);
            s( 9,24);
            s(10,40);
            s(13,17);
            s(14,18);
            s(21,25);
            s(22,26);
            s(23,53);
            s(27,57);
            s(29,33);
            s(30,34);
            s(31,61);
            s(37,41);
            s(38,42);
            s(39,54);
            s(43,58);
            s(45,49);
            s(46,50);
            s(47,62);

            s( 1, 4);
            s( 2, 8);
            s( 3,33);
            s( 6,12);
            s( 7,37);
            s(10,24);
            s(11,41);
            s(13,28);
            s(14,44);
            s(15,45);
            s(18,48);
            s(19,49);
            s(21,36);
            s(22,52);
            s(26,56);
            s(27,42);
            s(30,60);
            s(35,50);
            s(39,53);
            s(51,57);
            s(55,61);
            s(59,62);

            s( 2, 4);
            s( 3,17);
            s( 5, 6);
            s( 7,22);
            s( 8,16);
            s(11,25);
            s(12,20);
            s(14,28);
            s(15,29);
            s(18,32);
            s(19,33);
            s(23,37);
            s(26,40);
            s(30,44);
            s(31,45);
            s(34,48);
            s(35,49);
            s(38,52);
            s(41,56);
            s(43,51);
            s(46,60);
            s(47,55);
            s(57,58);
            s(59,61);

            s( 3,18);
            s( 7,21);
            s(11,32);
            s(15,30);
            s(17,26);
            s(19,25);
            s(22,36);
            s(23,29);
            s(27,41);
            s(31,52);
            s(33,48);
            s(34,40);
            s(37,46);
            s(38,44);
            s(42,56);
            s(45,60);

            s( 3,16);
            s( 7,20);
            s(11,24);
            s(15,21);
            s(17,18);
            s(19,34);
            s(22,28);
            s(23,38);
            s(25,40);
            s(26,32);
            s(27,33);
            s(29,44);
            s(30,36);
            s(31,37);
            s(35,41);
            s(39,52);
            s(42,48);
            s(43,56);
            s(45,46);
            s(47,60);

            s( 3, 9);
            s( 7,13);
            s(10,16);
            s(11,17);
            s(14,20);
            s(15,22);
            s(18,24);
            s(19,26);
            s(21,28);
            s(23,30);
            s(25,32);
            s(27,34);
            s(29,36);
            s(31,38);
            s(33,40);
            s(35,42);
            s(37,44);
            s(39,45);
            s(41,48);
            s(43,49);
            s(46,52);
            s(47,53);
            s(50,56);
            s(54,60);

            s( 3, 8);
            s( 7,10);
            s( 9,12);
            s(11,16);
            s(13,14);
            s(15,17);
            s(18,20);
            s(19,22);
            s(21,24);
            s(23,26);
            s(25,28);
            s(27,29);
            s(30,32);
            s(31,33);
            s(34,36);
            s(35,38);
            s(37,40);
            s(39,42);
            s(41,44);
            s(43,45);
            s(46,48);
            s(47,52);
            s(49,50);
            s(51,54);
            s(53,56);
            s(55,60);

            s( 3, 5);
            s( 6, 8);
            s( 7, 9);
            s(10,12);
            s(11,13);
            s(14,16);
            s(15,18);
            s(17,20);
            s(19,21);
            s(22,24);
            s(23,25);
            s(26,28);
            s(27,30);
            s(29,32);
            s(31,34);
            s(33,36);
            s(35,37);
            s(38,40);
            s(39,41);
            s(42,44);
            s(43,46);
            s(45,48);
            s(47,49);
            s(50,52);
            s(51,53);
            s(54,56);
            s(55,57);
            s(58,60);

            s( 3,4);
            s( 5,6);
            s( 7, 8);
            s( 9,10);
            s(11,12);
            s(13,14);
            s(15,16);
            s(17,18);
            s(19,20);
            s(21,22);
            s(23,24);
            s(25,26);
            s(27,28);
            s(29,30);
            s(31,32);
            s(33,34);
            s(35,36);
            s(37,38);
            s(39,40);
            s(41,42);
            s(43,44);
            s(45,46);
            s(47,48);
            s(49,50);
            s(51,52);
            s(53,54);
            s(55,56);
            s(57,58);
            s(59,60);
        }
    };

} // namespace Tools
