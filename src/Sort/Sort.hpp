#pragma once

#include <functional>

namespace Tools
{
    
    template<bool reverseQ = false, typename T, typename C = std::less<T>>
    void Sort( mptr<T> begin, mptr<T> end, C comp = C() )
    {
        // Use sorting networks for inputs of length <= 16; use std::sort otherwise.
        switch( std::distance( begin, end ) )
        {
            case 2:
            {
                return SortNet<2,reverseQ>()(begin,comp);
            }
            case 3:
            {
                return SortNet<3,reverseQ>()(begin,comp);
            }
            case 4:
            {
                return SortNet<4,reverseQ>()(begin,comp);
            }
            case 5:
            {
                return SortNet<5,reverseQ>()(begin,comp);
            }
            case 6:
            {
                return SortNet<6,reverseQ>()(begin,comp);
            }
            case 7:
            {
                return SortNet<7,reverseQ>()(begin,comp);
            }
            case 8:
            {
                return SortNet<8,reverseQ>()(begin,comp);
            }
            case 9:
            {
                return SortNet<9,reverseQ>()(begin,comp);
            }
            case 10:
            {
                return SortNet<10,reverseQ>()(begin,comp);
            }
            case 11:
            {
                return SortNet<11,reverseQ>()(begin,comp);
            }
            case 12:
            {
                return SortNet<12,reverseQ>()(begin,comp);
            }
            case 13:
            {
                return SortNet<13,reverseQ>()(begin,comp);
            }
            case 14:
            {
                return SortNet<14,reverseQ>()(begin,comp);
            }
            case 15:
            {
                return SortNet<15,reverseQ>()(begin,comp);
            }
            case 16:
            {
                return SortNet<16,reverseQ>()(begin,comp);
            }
            case 32:
            {
                return SortNet<32,reverseQ>()(begin,comp);
            }
            case 64:
            {
                return SortNet<64,reverseQ>()(begin,comp);
            }
            case 1:
            {
                return;
            }
            default:
            {
                return std::sort( begin, end, comp );
            }
        }
    }

} // namespace Tools

