#pragma once

namespace Tools
{
    
    template<typename T>
    TOOLS_FORCE_INLINE bool BinarySearch(
        cptr<T> sorted_list, const Size_T n,
        cref<T> value,
        mref<Size_T> pos
    )
    {
        if( n == 0 )
        {
            pos = 0;
            return false;
        }
        
        Size_T L = 0;
        Size_T R = n-1;
        
        if( value < sorted_list[L] )
        {
            pos = L;
            return false;
        }
        
        if( value > sorted_list[R] )
        {
            pos = R;
            return false;
        }
        
        while( L < R )
        {
            const Size_T C = R - (R-L)/static_cast<Size_T>(2);
            
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
        return sorted_list[L] == value;
        
    } // BinarySearch
    
    
}
