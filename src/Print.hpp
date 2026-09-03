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
    
    inline void print( std::string_view s )
    {
        const std::lock_guard<std::mutex> cout_lock( Tools::cout_mutex  );
        
#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
        const std::lock_guard<std::mutex> timer_lock( Timer::mutex );
        const Size_T len = 2 * (Timer::stack.size());
        std::string s_out (len + s.size(), ' ');
        std::copy_n( &s[0], s.size(), &s_out[len] );
        mma::print(s_out);
#else
        std::cout << s << std::endl;
#endif
    }
    
    template<bool printQ = true, Stringy T>
    inline void print( const T & x )
    {
        if constexpr ( printQ )
        {
            print(std::string_view(x));
        }
        else
        {
            (void)x;
        }
    }
    
    template<bool printQ = true, typename T, typename ...Args>
    inline void print( T && x, const Args &... args )
    {
        if constexpr ( printQ )
        {
            // We forward the first argument's type because it could be an OutString &&
            
            print(
                OutString::FromMisc(std::forward<T>(x),args...).View()
            );
        }
        else
        {
            Void{x,args...};
        }
    }
    
    
    template<bool printQ = true, Size_T align = 0>
    inline void valprint( std::string_view s, std::string_view value )
    {
        if constexpr ( printQ )
        {
            if constexpr ( align == Size_T{0} )
            {
                print( s, " = ", value );
            }
            else
            {
                print( ct_string<align+Size_T{1}>(), s, " = ", value );
            }
        }
        else
        {
            Void{s,value};
        }
        
//        const Size_T len = (align > Size_T{0}) ? std::max(s.size(),align) : s.size();
//
//        std::string s_out (len + Size_T{3} + value.size(),' ');
//        std::copy_n( s.begin(), s.size(), s_out.begin() );
//        s_out[len+Size_T{1}] = '=';
//        std::copy_n( value.begin(), value.size(), &s_out[len + Size_T{3}] );
//        print(s_out);
    }
    
    template<bool printQ = true, Size_T align = 0, typename T>
    inline void valprint( std::string_view s, const T & value )
    {
        if constexpr ( printQ )
        {
            if constexpr ( align == Size_T{0} )
            {
                // Calling ToString here is not ideal, but it is robust.
                print( s, " = ", ToString(value) );
            }
            else
            {
                // Calling ToString here is not ideal, but it is robust.
                print( ct_string<align+Size_T{1}>(), s, " = ", ToString(value) );
            }
        }
        else
        {
            Void{s,value};
        }
//        const Size_T len = (align > Size_T{0}) ? std::max(s.size(),align) : s.size();
//        
//        std::string s_out (len + Size_T{3}, ' ');
//        std::copy_n( s.begin(), s.size(), s_out.begin() );
//        s_out[len+Size_T{1}] = '=';
//        s_out += ToString(value);
//        print(s_out);
    }
    
    

} // namespace Tools


#define TOOLS_DUMP(x) Tools::valprint( std::string_view(#x), x )

#define TOOLS_MEM_DUMP_STRING(x) "\"" + std::string(#x) + "\" -> " + Tools::ToMathematicaString( static_cast<double>((x).AllocatedByteCount()) )

#define TOOLS_MEM_DUMP(x) Tools::print( TOOLS_MEM_DUMP_STRING(x) )
