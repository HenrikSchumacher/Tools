#pragma once

namespace Tools
{
    template<typename I, typename J >
    inline constexpr void set_bit( I & x, const J pos, const bool digit )
    {
        const I mask = ~(static_cast<I>(1) << pos);
        
        x = (x & mask) | (static_cast<I>(digit) << pos);
    }
    
    template<typename I, typename J>
    inline constexpr bool get_bit( const I & x, const J pos )
    {
        const I mask = ( static_cast<I>(1) << pos );
        
        return ((x & mask) != 0);
    }
    
    template<typename I, typename J >
    inline constexpr void activate_bit( I & x, const J pos )
    {
        x |= (static_cast<I>(1) << pos);
    }
    
    template<typename I, typename J >
    inline constexpr void deactivate_bit( I & x, const J pos )
    {
        x &= ~(static_cast<I>(1)<<pos);
    }
    
    
    
    
    template<typename I, typename J>
    inline constexpr void set_byte( I & x, const J pos, const I b )
    {
        const I mask = ( ~static_cast<I>(0xFF) ) << (pos * 8);
        
        x = (x & mask) | (static_cast<I>(b) << (pos * 8));
    }
    
    template<typename I, typename J>
    inline constexpr I get_byte( const I & x, const J pos )
    {
        constexpr I mask = static_cast<I>(0xFF);
        
        return ( x >> (pos * 8) ) & mask;
    }

} // namespace Tools
