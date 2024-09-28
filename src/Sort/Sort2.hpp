#pragma once

#include <functional>

namespace Tools
{
    
    template<typename CompSwap_T, typename Int>
    void Sort2( Int n, CompSwap_T s )
    {
        static_assert(IntQ<Int>);
        
        // Use sorting networks for inputs of length <= 16; use std::sort otherwise.
        switch( n )
        {
            case 2:
            {
                SortNet2<2>()(s);
                break;
            }
            case 3:
            {
                SortNet2<3>()(s);
                break;
            }
            case 4:
            {
                SortNet2<4>()(s);
                break;
            }
            case 5:
            {
                SortNet2<5>()(s);
                break;
            }
            case 6:
            {
                SortNet2<6>()(s);
                break;
            }
            case 7:
            {
                SortNet2<7>()(s);
                break;
            }
            case 8:
            {
                SortNet2<8>()(s);
                break;
            }
            case 9:
            {
                SortNet2<9>()(s);
                break;
            }
            case 10:
            {
                SortNet2<10>()(s);
                break;
            }
            case 11:
            {
                SortNet2<11>()(s);
                break;
            }
            case 12:
            {
                SortNet2<12>()(s);
                break;
            }
            case 13:
            {
                SortNet2<13>()(s);
                break;
            }
            case 14:
            {
                SortNet2<14>()(s);
                break;
            }
            case 15:
            {
                SortNet2<15>()(s);
                break;
            }
            case 16:
            {
                SortNet2<16>()(s);
                break;
            }
            case 32:
            {
                SortNet2<32>()(s);
                break;
            }
            case 64:
            {
                SortNet2<64>()(s);
                break;
            }
            case 1:
            {
                break;
            }
            default:
            {
//                std::sort( begin, end, comp );
                eprint("Sort2: - not implemented.");
                break;
            }
        }
    }

} // namespace Tools

