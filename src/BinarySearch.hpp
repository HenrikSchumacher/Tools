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
    
    
    
    
    
    /*!@brief Returns the first position `pos` in the sorted input array `sorted_list` such that `sorted_list[pos] >= value`. If such a position exists, then `std::pair(true,pos)`is returned. Otherwise `std::pair(false,n)` is returned.
     */
    template<typename T, typename Int, typename CMP = std::less>
    TOOLS_FORCE_INLINE Int FindFirstGreaterEqual(
        cptr<T> sorted_list, const Int n, cref<T> value, CMP && cmp = CMP()
    )
    {
        static_assert(IntQ<Int>,"");
        
        if( n == Int(0) )
        {
            return std::pair(false,n);
        }
        
        Int L = Int(0);
        Int R = n - Int(1);
        
        if( !cmp(sorted_list[L],value)  ) // sorted_list[L] >= value
        {
            return std::pair(true,L);
        }
        
        if( cmp(sorted_list[R],value) ) // value < sorted_list[R]
        {
            return std::pair(false,n);
        }
        
        // Invariants: sorted_list[L] < value <= sorted_list[R]
        
        while( L + Int(1) < R )
        {
            const Int C = R - (R-L)/static_cast<Int>(2);
            
            
            if( cmp(value,sorted_list[C]) ) // value < sorted_list[C]
            {
                L = C;
            }
            else // value >= sorted_list[C]
            {
                R = C;
            }
        }
        
        return std::pair(true,R);
        
    } // FindFirstGreaterEqual
    
    
    /*!@brief Returns the last position `pos` in the sorted input array `sorted_list` such that `sorted_list[pos] M value`. If such a position exists, then `std::pair(true,pos)`is returned. Otherwise `std::pair(false,0)` is returned.
     */
    template<typename T, typename Int, typename CMP = std::less>
    TOOLS_FORCE_INLINE Int FindLastLess(
        cptr<T> sorted_list, const Int n, cref<T> value, CMP && cmp = CMP()
    )
    {
        static_assert(IntQ<Int>,"");
        
        if( n == Int(0) )
        {
            return std::pair(false,Int(0));
        }
        
        Int L = Int(0);
        Int R = n - Int(1);
        
        if( !cmp(sorted_list[L],value)  ) // sorted_list[L] >= value
        {
            return std::pair(false,Int(0));
        }
        
        if( cmp(sorted_list[R],value) ) // sorted_list[R] < value
        {
            return std::pair(true,R);
        }
        
        // Invariants: sorted_list[L] < value <= sorted_list[R]
        
        while( L + Int(1) < R )
        {
            const Int C = R - (R-L)/static_cast<Int>(2);
            
            if( cmp(value,sorted_list[C]) ) // value < sorted_list[C]
            {
                L = C;
            }
            else // value >= sorted_list[C]
            {
                R = C;
            }
        }
        
        return std::pair(true,L);
        
    } // FindLastLess
    
    
}
