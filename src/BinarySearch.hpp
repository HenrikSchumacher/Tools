#pragma once

namespace Tools
{
    /*!@brief Returns the largest position `pos` in the input array such that `sorted_list[pos] <= value`. If `value` is found, then `true` is returned. Otherwise `false` is returned.
     */
    template<typename T, typename Int>
    TOOLS_FORCE_INLINE bool BinarySearch(
        cptr<T> sorted_list, const Int n, cref<T> value, mref<Int> pos
    )
    {
        static_assert(IntQ<Int>,"");
        
        if( n == Int(0) )
        {
            pos = Int(0);
            return false;
        }
        
        Int L = Int(0);
        Int R = n - Int(1);
        
        if( value <= sorted_list[L] )
        {
            pos = L;
            return (value == sorted_list[L]);
        }
        
        if( value >= sorted_list[R] )
        {
            pos = R;
            return (value == sorted_list[R]);
        }
        
        while( L < R )
        {
            const Int C = R - (R-L)/static_cast<Int>(2);
            
            if( value < sorted_list[C] )
            {
                R = C - 1;
            }
            else
            {
                L = C;
            }
        }
        
        pos = L;
        return (sorted_list[L] == value);
        
    } // BinarySearch
    
    
}
