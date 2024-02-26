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
                SortNet<2,reverseQ>()(begin,comp);
                break;
            }
            case 3:
            {
                SortNet<3,reverseQ>()(begin,comp);
                break;
            }
            case 4:
            {
                SortNet<4,reverseQ>()(begin,comp);
                break;
            }
            case 5:
            {
                SortNet<5,reverseQ>()(begin,comp);
                break;
            }
            case 6:
            {
                SortNet<6,reverseQ>()(begin,comp);
                break;
            }
            case 7:
            {
                SortNet<7,reverseQ>()(begin,comp);
                break;
            }
            case 8:
            {
                SortNet<8,reverseQ>()(begin,comp);
                break;
            }
            case 9:
            {
                SortNet<9,reverseQ>()(begin,comp);
                break;
            }
            case 10:
            {
                SortNet<10,reverseQ>()(begin,comp);
                break;
            }
            case 11:
            {
                SortNet<11,reverseQ>()(begin,comp);
                break;
            }
            case 12:
            {
                SortNet<12,reverseQ>()(begin,comp);
                break;
            }
            case 13:
            {
                SortNet<13,reverseQ>()(begin,comp);
                break;
            }
            case 14:
            {
                SortNet<14,reverseQ>()(begin,comp);
                break;
            }
            case 15:
            {
                SortNet<15,reverseQ>()(begin,comp);
                break;
            }
            case 16:
            {
                SortNet<16,reverseQ>()(begin,comp);
                break;
            }
            case 32:
            {
                SortNet<32,reverseQ>()(begin,comp);
                break;
            }
            case 64:
            {
                SortNet<64,reverseQ>()(begin,comp);
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

