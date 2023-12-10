#pragma once

namespace Tools
{
    
    template<int return_lower = true, typename T>
    force_inline bool BinarySearch( cptr<T> sorted_list, const Size_T n, cref<T> value, mref<Size_T> pos )
    {
        Size_T a = 0;
        Size_T b = n;
        
        T f_a = sorted_list[a];
        T f_b = sorted_list[b];
        
        if( value < f_a )
        {
            pos = a;
            return false;
        }
        
        if( value >= f_b )
        {
            pos = b;
            return false;
        }
        
        while( b > a + 1 )
        {
            const Real c = a + (b-a) / 2;
            
            const Real f_c = sorted_list[c];
            
            if( f_c > value )
            {
                b   = c;
                f_b = f_c;
            }
            else
            {
                a   = c;
                f_a = f_c;
            }
        }
        
        if constexpr ( return_lower )
        {
            pos = a;
        }
        else
        {
            pos = b;
        }
        
        return true;
        
    } // BinarySearch
    
    
}
