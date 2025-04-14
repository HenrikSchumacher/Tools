#pragma once

namespace Tools
{
    template<typename F, typename Int>
    Int FindFirst( F && f, const Int a, const Int b, const Int inc = Int(1) )
    {
        for( Int i = a; i < b; i += inc )
        {
            if( f(i) )
            {
                return i;
            }
        }
        
        return b;
    }
    
    template<typename F, typename Int>
    Int FindLast( F && f, const Int a, const Int b )
    {
        for( Int i = b; i --> a;  )
        {
            if( f(i) )
            {
                return i;
            }
        }
        
        return b;
    }
    
    template<typename F, typename Int>
    bool ForAllQ( F && f, const Int a, const Int b, const Int inc = Int(1) )
    {
        for( Int i = a; i < b; i += inc )
        {
            if( !f(i) )
            {
                return false;
            }
        }
        
        return true;
    }
    
    template<typename F, typename Int>
    bool ExistsQ( F && f, const Int a, const Int b, const Int inc = Int(1) )
    {
        for( Int i = a; i < b; i += inc )
        {
            if( f(i) )
            {
                return true;
            }
        }
        
        return false;
    }
}
