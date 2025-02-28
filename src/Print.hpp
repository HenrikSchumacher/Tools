#pragma once

namespace Tools
{
    static std::mutex cout_mutex;
    static std::mutex cerr_mutex;
    
    // We need it here because print might need knowledge of it when running in Mathematica.
    using Clock = std::chrono::high_resolution_clock;
    using Time  = std::chrono::time_point<Clock>;
    
    namespace Timer
    {
        static std::vector<Time> stack;
        static std::mutex mutex;
    }
    
    inline void print( const std::string & s )
    {
        const std::lock_guard<std::mutex>  cout_lock( Tools::cout_mutex  );
        
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
        const std::lock_guard<std::mutex> timer_lock( Timer::mutex );
        
        mma::print( std::string( 2 * (Timer::stack.size()), ' ') + s );
#else
        std::cout << s << std::endl;
#endif
    }
    
    template<Size_T N>
    inline void print( const ct_string<N> & s )
    {
        print( s.data() );
    }
    
    template<Size_T align = 0, typename T>
    inline void valprint( const std::string & s, const T & value)
    {
        if constexpr ( align > 0 )
        {
            const Size_T len = s.size();
            
            if( len <= align )
            {
                std::string s_out = (ct_spaces<align> + " = ") + COND(FloatQ<T>,ToStringFPGeneral(value),ToString(value));
                
                std::copy_n( s.begin(), len, s_out.begin() );
                
                print(s_out);
            }
            else
            {
                print( s + " = " + COND(FloatQ<T>,ToStringFPGeneral(value),ToString(value)) );
            }
        }
        else
        {
            print( s + " = " + COND(FloatQ<T>,ToStringFPGeneral(value),ToString(value)) );
        }
    }
    
    template<Size_T align = 0, typename T>
    inline void valprint( const std::string & s, const std::string & value)
    {
        if constexpr ( align > 0 )
        {
            const Size_T len = s.size();
            
            print( s + " = " + value );
            
            if( len <= align )
            {
                std::string s_out = (ct_spaces<align> + " = ") + value;
                
                std::copy_n( s.begin(), len, s_out.begin() );
                
                print(s_out);
            }
            else
            {
                print( s + " = " + value );
            }
        }
        else
        {
            print( s + " = " + value );
        }
    }

} // namespace Tools


#define TOOLS_DUMP(x) Tools::valprint( std::string(#x), x );

#define TOOLS_MEM_DUMP_STRING(x) "\"" + std::string(#x) + "\" -> " + ToString((x).AllocatedByteCount())

#define TOOLS_MEM_DUMP(x) Tools::print( TOOLS_MEM_DUMP_STRING(x) )
