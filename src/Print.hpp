#pragma once

namespace Tools
{
    std::mutex cout_mutex;
    std::mutex cerr_mutex;
    std::mutex timer_mutex;
    
    using Clock = std::chrono::steady_clock;
    using Time  = std::chrono::time_point<Clock>;
    
    force_inline double Duration( const Time & start_time, const Time & stop_time )
    {
        return std::chrono::duration<double>(stop_time - start_time).count();
    }
    
    namespace Timer
    {
        std::vector<Time> time_stack;
    }


    inline void print( const std::string & s )
    {
        const std::lock_guard<std::mutex>  cout_lock( Tools::cout_mutex  );
        
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
        const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
        
        mma::print( std::string( 2 * (Timer::time_stack.size()), ' ') + s );
#else
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
    
    inline void tic(const std::string & s)
    {
        {
            const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
            
            Timer::time_stack.push_back(Clock::now());
        }
        
        print( s + "..." );
    }

    inline double toc(const std::string & s)
    {
        double duration (0);
    
        
        if (!Timer::time_stack.empty())
        {
            {
                const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
                duration = Tools::Duration( Timer::time_stack.back(), Clock::now() );
            }
                print( std::to_string(duration) + " s.");
            {
                const std::lock_guard<std::mutex> timer_lock( Tools::timer_mutex );
                Timer::time_stack.pop_back();
            }
        }
        else
        {
            print("Unmatched toc detected. Label =  " + s);
        }
        
        return duration;
    }

} // namespace Tools


#define dump(x) Tools::valprint( std::string(#x), x );
