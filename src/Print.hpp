#pragma once

namespace Tools
{
    using Clock = std::chrono::steady_clock;
    using Time  = std::chrono::time_point<Clock>;
    
    float Duration( const Time & start_time, const Time & stop_time )
    {
        return std::chrono::duration<float>(stop_time - start_time).count()
    }
    
    namespace Timer
    {
        std::vector<Time> time_stack;
    }

#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
    inline void print( const std::string & s )
    {
        #pragma omp critical(cout)
        {
            mma::print( std::string( 2 * (Timer::time_stack.size()), ' ') + s );
        }
    }
#else
    inline void print( const std::string & s )
    {
        #pragma omp critical(cout)
        {
            std::cout << s << std::endl;
        }
    }
#endif
    
    template<typename T>
    inline void valprint( const std::string & s, const T & value)
    {
        using namespace std;
        print( s + " = " + ToString(value) );
    }

    template<typename T>
    inline void valprint( const std::string & s, const T & value, const int p)
    {
        print( s + " = " + ToString(value, p) );
    }
    
    inline void valprint( const std::string & s, const std::string & value)
    {
        using namespace std;
        print( s + " = " + value );
    }
    
#define dump(x) valprint( std::string(#x), x );
    
    inline void tic(const std::string & s)
    {
        print(s + "...");
        #pragma omp critical(timers)
        {
            Timer::time_stack.push_back(Clock::now());
        }
    }

    inline float toc(const std::string & s)
    {
        float duration = 0;
        #pragma omp critical(timers)
        {
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
        }
        return duration;
    }

} // namespace Tools
