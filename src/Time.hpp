#pragma once

namespace Tools
{
    using Clock = std::chrono::high_resolution_clock;
    using Time  = std::chrono::time_point<Clock>;
    
    TOOLS_FORCE_INLINE double Duration( const Time & start_time, const Time & stop_time )
    {
        return std::chrono::duration<double>(stop_time - start_time).count();
    }
    
    class TimePoint
    {
        std::chrono::time_point<Clock> t;
        
    public:
        
        friend class TimeInterval;
        
        TimePoint( bool flag )
        {
            (void)flag;
            
            Now();
        }
        
        TimePoint() = default;
        
        ~TimePoint() = default;
        
        void Now()
        {
            t = Clock::now();
        }
        
        inline friend double Duration( cref<TimePoint> a, cref<TimePoint> b )
        {
            return std::chrono::duration<double>(b.t - a.t).count();
        }
    };
    
    class TimeInterval
    {
        TimePoint t [2];
        
    public:
        
        
        TimeInterval( bool flag )
        {
            (void)flag;
            Tic();
        }
        
        TimeInterval() = default;
        
        ~TimeInterval() = default;
        
        
        cref<TimePoint> operator[]( bool i )
        {
            return t[i];
        }
        
        template<bool do_itQ = true>
        void Tic()
        {
            if constexpr ( do_itQ )
            {
                t[0].Now();
            }
        }
        
        template<bool do_itQ = true>
        void Toc()
        {
            if constexpr ( do_itQ )
            {
                t[1].Now();
            }
        }
        
        double Duration() const
        {
            return std::chrono::duration<double>(t[1].t - t[0].t).count();
        }
    };
    
    
    
    static std::mutex timer_mutex;
    
    namespace Timer
    {
        static std::vector<Time> time_stack;
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
        double duration = 0;
        
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
