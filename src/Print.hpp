#pragma once

namespace Tools
{
    std::mutex cout_mutex;
    std::mutex cerr_mutex;
    std::mutex timer_mutex;
    
    using Clock = std::chrono::steady_clock;
    using Time  = std::chrono::time_point<Clock>;
    
    force_inline float Duration( const Time & start_time, const Time & stop_time )
    {
        return std::chrono::duration<float>(stop_time - start_time).count();
    }
    
    namespace Timer
    {
        std::vector<Time> time_stack;
    }


    inline void print( const std::string & s )
    {
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
        const std::lock_guard<std::mutex>  cout_lock( Tools::cout_mutex  );
        const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
        
        mma::print( std::string( 2 * (Timer::time_stack.size()), ' ') + s );
#else
        const std::lock_guard<std::mutex> cout_lock( Tools::cout_mutex );
        
        std::cout << s << std::endl;
#endif
    }
    
    template<typename T>
    inline void valprint( const std::string & s, const T & value)
    {
        print( s + " = " + ToString(value) );
    }

    template<typename T>
    inline void valprint( const std::string & s, const T & value, const int p)
    {
        print( s + " = " + ToString(value, p) );
    }
    
    inline void valprint( const std::string & s, const std::string & value)
    {
        print( s + " = " + value );
    }
    
#define dump(x) valprint( std::string(#x), x );
    
    inline void tic(const std::string & s)
    {
        const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
        
        Timer::time_stack.push_back(Clock::now());
    }

    inline float toc(const std::string & s)
    {
        float duration (0);
        
        const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
        
        if (!Timer::time_stack.empty())
        {
            duration = Duration( Timer::time_stack.back(), Clock::now() );
            Timer::time_stack.pop_back();
            
            print( std::to_string(duration) + " s.");
            
        }
        else
        {
            print("Unmatched toc detected. Label =  " + s);
        }
        
        return duration;
    }

} // namespace Tools
