namespace Tools
{
    template<typename I, typename J >
    inline void set_bit( I & x, const J pos, const bool digit )
    {
        const I mask = ~(static_cast<I>(1) << pos);
        
        x = (x & mask) | (static_cast<I>(digit) << pos);
    }
    
    template<typename I, typename J>
    inline bool get_bit( I & x, const J pos )
    {
        const I mask = ( static_cast<I>(1) << pos );
        
        return ((x & mask) != 0);
    }
    
    template<typename I, typename J >
    inline void activate_bit( I & x, const J pos )
    {
        x |= (static_cast<I>(1) << pos);
    }
    
    template<typename I, typename J >
    inline void deactivate_bit( I & x, const J pos )
    {
        x &= ~(static_cast<I>(1)<<pos);
    }

} // namespace Tools
