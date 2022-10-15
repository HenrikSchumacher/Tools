#pragma once

namespace Tools
{
    namespace Timers
    {
        std::vector<std::chrono::time_point<std::chrono::steady_clock>> time_stack;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::chrono::time_point<std::chrono::steady_clock> stop_time;
    }

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
