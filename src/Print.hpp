#pragma once

namespace Tools
{    
    namespace Timers
    {
        std::vector<std::chrono::time_point<std::chrono::steady_clock>> time_stack;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::chrono::time_point<std::chrono::steady_clock> stop_time;
    }

#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
    inline void print( const std::string & s )
    {
        #pragma omp critical(cout)
        {
            mma::print( std::string( 2 * (Timers::time_stack.size()), ' ') + s );
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
            Timers::time_stack.push_back(std::chrono::steady_clock::now());
        }
    }

    inline double toc(const std::string & s)
    {
        double duration = 0.;
        #pragma omp critical(timers)
        {
            if (!Timers::time_stack.empty())
            {
                auto start_time = Timers::time_stack.back();
                auto stop_time = std::chrono::steady_clock::now();
                duration = std::chrono::duration<double>(stop_time - start_time).count();
                Timers::time_stack.pop_back();
                
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
