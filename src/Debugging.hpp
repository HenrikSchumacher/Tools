#pragma once

namespace Tools
{
    // From https://stackoverflow.com/a/49658950/8248900.
    template<IntQ Int_1, IntQ Int_0>
    TOOLS_FORCE_INLINE constexpr Int_1 int_cast( const Int_0 n )
    {
        if( !std::in_range<Int_1>( n ) )
        {
            eprint(std::string("int_cast<") + TypeName<Int_1> + "," + TypeName<Int_0> + "> reports integer overflow for n = " + ToString(n) + ".");
            
            return 0;
        }
        
        return static_cast<Int_1>(n);
    }
    
    
    template<typename T>
    TOOLS_FORCE_INLINE void assert_positive( const T x )
    {
#ifdef TOOLS_DEBUG
        if constexpr ( std::numeric_limits<T>::is_signed )
        {
            if( x <= static_cast<T>(0) )
            {
                eprint(std::string("assert_positive failed in function in ") + std::string(__FILE__) + " at line "+ ToString(__LINE__)+".");
            }
        }
#else
        (void)x;
#endif
    }
    
    
    void Check_is_pointer()
    {
        
        print("\nCheck_is_pointer()\n");
#ifdef TOOLS_NO_RESTRICT
        print("TOOLS_NO_RESTRICT is defined.\n");
#else
        print("TOOLS_NO_RESTRICT is undefined.\n");
#endif
        
        TOOLS_DUMP( std::is_pointer<bool *>::value );
        TOOLS_DUMP( std::is_pointer<bool * const>::value );
        TOOLS_DUMP( std::is_pointer<const bool * const>::value );
        print("");
        TOOLS_DUMP( std::is_pointer<bool * restrict>::value );
        TOOLS_DUMP( std::is_pointer<bool * const restrict>::value );
        TOOLS_DUMP( std::is_pointer<const bool * const restrict>::value );
        print("");
        TOOLS_DUMP( std::is_pointer<bool * restrict>::value );
        TOOLS_DUMP( std::is_pointer<bool * restrict const>::value );
        TOOLS_DUMP( std::is_pointer<const bool * restrict const>::value );
    }
    
} // namespace Tools

