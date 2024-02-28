#pragma once

namespace Tensors
{
    // From https://stackoverflow.com/a/49658950/8248900.
    template<typename Int_1, typename Int_0>
    force_inline constexpr Int_1 int_cast( const Int_0 n )
    {
        ASSERT_INT(Int_1);
        ASSERT_INT(Int_0);
        
        typedef std::numeric_limits<Int_1> Lim_1;
        typedef std::numeric_limits<Int_0> Lim_0;
        
        constexpr bool positive_overflow_possible = Lim_1::max() < Lim_0::max();
        constexpr bool negative_overflow_possible =
        Lim_0::is_signed
        ||
        (Lim_1::lowest() > Lim_0::lowest());
        
        // unsigned <-- unsigned
        if constexpr ( (! Lim_1::is_signed) && (! Lim_0::is_signed) )
        {
            if constexpr (positive_overflow_possible )
            {
#ifdef TOOLS_DEBUG
                if ( n > Lim_1::max() )
                {
                    eprint(std::string("int_cast<") + TypeName<Int_1> + "> reports integer overflow n > " + ToString(Lim_1::max()) + "  for n = " + ToString(n) + " of type " + TypeName<Int_0> + ".");
                }
#endif
            }
        }
        
        // unsigned <-- signed
        if constexpr( (! Lim_1::is_signed) && Lim_0::is_signed )
        {
            if constexpr (positive_overflow_possible )
            {
#ifdef TOOLS_DEBUG
                if( n > Lim_1::max() )
                {
                    eprint(std::string("int_cast<") + TypeName<Int_1> + "> reports integer overflow n > " + ToString(Lim_1::max()) + "  for n = " + ToString(n) + " of type " + TypeName<Int_0> + ".");
                }
#endif
            }
            
            if constexpr (negative_overflow_possible )
            {
#ifdef TOOLS_DEBUG
                if ( n < 0 )
                {
                    eprint(std::string("int_cast<") + TypeName<Int_1> + "> reports integer underflow n < 0 for n = " + ToString(n) + " of type " + TypeName<Int_0> + ".");
                }
#endif
            }
            
        }
        
        // signed <-- unsigned
        if constexpr ( Lim_1::is_signed && !Lim_0::is_signed )
        {
            if constexpr ( positive_overflow_possible )
            {
#ifdef TOOLS_DEBUG
                if( n > Lim_1::max() )
                {
                    eprint(std::string("int_cast<") + TypeName<Int_1> + "> reports integer overflow n > " + ToString(Lim_1::max()) + "  for n = " + ToString(n) + " of type " + TypeName<Int_0> + ".");
                }
#endif
            }
        }
        
        // signed <-- signed
        if constexpr ( Lim_1::is_signed && Lim_0::is_signed )
        {
            if constexpr( positive_overflow_possible )
            {
#ifdef TOOLS_DEBUG
                if( n > Lim_1::max() )
                {
                    eprint(std::string("int_cast<") + TypeName<Int_1> + "> reports integer overflow n > " + ToString(Lim_1::max()) + "  for n = " + ToString(n) + " of type " + TypeName<Int_0> + ".");
                }
#endif
            }
            
            if constexpr (negative_overflow_possible )
            {
#ifdef TOOLS_DEBUG
                if ( n < Lim_1::lowest() )
                {
                    eprint(std::string("int_cast<") + TypeName<Int_1> + "> reports integer underflow n < " + ToString(Lim_1::lowest()) + "  for n = " + ToString(n) + " of type " + TypeName<Int_0> + ".");
                }
#endif
            }
        }
        
        return static_cast<Int_1>(n);
    }
    
    
    template<typename T>
    force_inline void assert_positive( const T x )
    {
#ifdef TOOLS_DEBUG
        if constexpr ( std::numeric_limits<T>::is_signed )
        {
            if( x <= static_cast<T>(0) )
            {
                eprint(std::string("assert_positive failed in function in ") + std::string(__FILE__) + " at line "+ ToString(__LINE__)+".");
            }
        }
#endif
    }
    
} // namespace Tensors

