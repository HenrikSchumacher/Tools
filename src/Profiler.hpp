#pragma once

#include <mutex>

namespace Tools
{
    namespace Profiler
    {
        std::mutex prof_mutex;
        std::mutex log_mutex;
        
        
        std::ofstream prof ("./Tools_Profile.tsv");
        std::ofstream log  ("./Tools_Log.txt");

        Time init_time = Clock::now();
        std::vector<Time> time_stack;
        std::vector<std::string> tag_stack(1,"root");
        std::vector<int> parent_stack (1, -0);
        std::vector<int> id_stack (1,0);
        int id_counter = 0;
        
        
        inline void Clear(const std::string & dir, const bool append = false)
        {
            const std::lock_guard<std::mutex> prof_lock( prof_mutex );
            const std::lock_guard<std::mutex> log_lock( log_mutex );
            
            std::filesystem::path profile_filename = dir;
            profile_filename /= "Tools_Profile.tsv";
            Profiler::prof.close();
            if( append )
            {
                Profiler::prof.open(profile_filename.string(), std::ios_base::app);
            }
            else
            {
                Profiler::prof.open(profile_filename.string());
            }
            
            print( std::string("Profile will be written to ") + profile_filename.string() + ".");
            
            
            Profiler::log << std::setprecision(16);
            
            std::filesystem::path log_filename = dir;
            log_filename /= "Tools_Log.txt";
            Profiler::log.close();

            if( append )
            {
                Profiler::log.open(log_filename.string(), std::ios_base::app);
            }
            else
            {
                Profiler::log.open(log_filename.string());
            }

            print( std::string("Log     will be written to ") + log_filename.string() + "." );
            Profiler::log << std::setprecision(16);
    //        Profiler::prof << "ID" << "\t" << "Tag" << "\t" << "From" << "\t" << "Tic" << "\t" << "Toc" << "\t" << "Duration" << "\t" << "Depth" << "\n";
            Profiler::init_time = Clock::now();
            Profiler::time_stack.clear();
            Profiler::parent_stack.push_back(0.);
            Profiler::tag_stack.clear();
            Profiler::tag_stack.push_back("root");
            Profiler::parent_stack.clear();
            Profiler::parent_stack.push_back(-1);
            Profiler::id_stack.clear();
            Profiler::id_stack.push_back(0);
            Profiler::id_counter = 0;
        }
        
    } // namespace Profiler
    
    
// Print functions that are always active and print to the log file.
    
    inline void logprint( const std::string & s )
    {
        const std::lock_guard<std::mutex> lock( Profiler::log_mutex );
        Profiler::log << s << "\n" << std::endl;
    }

    template<typename T>
    inline void logvalprint( const std::string & s, const T & value)
    {
        logprint( s + " = " + ToString(value) );
    }

    template<typename T>
    inline void logvalprint( const std::string & s, const T & value, const int p)
    {
        logprint( s + " = " + ToString(value, p) );
    }
    
    inline void logvalprint( const std::string & s, const std::string & value)
    {
        logprint( s + " = " + value );
    }
    
    inline void eprint( const std::string & s )
    {
        std::string msg ( std::string("ERROR: ") + s );
        
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)

        print( msg );
#endif
        const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
        std::cerr << msg << std::endl;
        logprint( msg );
    }

    inline void wprint( const std::string & s )
    {
        print(    std::string("WARNING: ") + s );
        logprint( std::string("WARNING: ") + s );
    }
    
#define logdump(x) logvalprint( std::string(#x), x );
    
    
    
    
// Print functions that are always active and print to the log file.
    
    inline void logprint( const std::string & s )
    {
        const std::lock_guard<std::mutex> lock( Profiler::log_mutex );
        Profiler::log << s << "\n" << std::endl;
    }

    template<typename T>
    inline void logvalprint( const std::string & s, const T & value)
    {
        logprint( s + " = " + ToString(value) );
    }

    template<typename T>
    inline void logvalprint( const std::string & s, const T & value, const int p)
    {
        logprint( s + " = " + ToString(value, p) );
    }
    
    inline void logvalprint( const std::string & s, const std::string & value)
    {
        logprint( s + " = " + value );
    }
    
    inline void eprint( const std::string & s )
    {
        std::string msg ( std::string("ERROR: ") + s );
        
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)

        print( msg );
#endif
        const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
        std::cerr << msg << std::endl;
        logprint( msg );
    }

    inline void wprint( const std::string & s )
    {
        print(    std::string("WARNING: ") + s );
        logprint( std::string("WARNING: ") + s );
    }
    
#define logdump(x) logvalprint( std::string(#x), x );
    
// Print functions that are only active if macro TOOLS_ENABLE_PROFILER is defined.
    
    inline void pprint( const std::string & s )
    {
#ifdef TOOLS_ENABLE_PROFILER
        const std::lock_guard<std::mutex> lock( Profiler::log_mutex );
        Profiler::log << s << "\n" << std::endl;
#endif
    }

    template<typename T>
    inline void pvalprint( const std::string & s, const T & value)
    {
        pprint( s + " = " + ToString(value) );
    }

    template<typename T>
    inline void pvalprint( const std::string & s, const T & value, const int p)
    {
        pprint( s + " = " + ToString(value, p) );
    }
    
    inline void pvalprint( const std::string & s, const std::string & value)
    {
        pprint( s + " = " + value );
    }
    
#define pdump(x) pvalprint( std::string(#x), x );
    
    inline void ptic(const std::string & tag)
    {
#ifdef TOOLS_ENABLE_PROFILER
        const std::lock_guard<std::mutex> prof_lock( Profiler::prof_mutex );
        
        Profiler::time_stack.push_back(Clock::now());
        Profiler::parent_stack.push_back(Profiler::id_stack.back());
        Profiler::tag_stack.push_back(tag);
        Profiler::id_stack.push_back(++Profiler::id_counter);
        
        float start_time = Duration( Profiler::init_time, Profiler::time_stack.back() );
        
        const std::lock_guard<std::mutex> log_lock( Profiler::log_mutex );
        
        Profiler::log
            << std::string( 2 * (Profiler::time_stack.size()), ' ' )
            << Profiler::tag_stack.back() << "\t started at \t" << start_time << "\n"
            << std::endl;
#endif
    }
    
    inline void ptoc(const std::string & tag)
    {
#ifdef TOOLS_ENABLE_PROFILER
        const std::lock_guard<std::mutex> prof_lock( Profiler::prof_mutex );
        if( !Profiler::tag_stack.empty() )
        {
            if( tag == Profiler::tag_stack.back() )
            {
                float start_time = Duration( Profiler::init_time, Profiler::time_stack.back() );
                float stop_time  = Duration( Profiler::init_time, Clock::now() );
                
                Profiler::prof
                    << Profiler::id_stack.back() <<  "\t"
                    << Profiler::tag_stack.back() << "\t"
                    << Profiler::parent_stack.back() << "\t"
                    << start_time << "\t"
                    << stop_time << "\t"
                    << stop_time-start_time << "\t"
                    << Profiler::tag_stack.size()-1
                    << std::endl;
                
                const std::lock_guard<std::mutex> log_lock( Profiler::log_mutex );
                Profiler::log
                    << std::string( 2 * (Profiler::time_stack.size()), ' ' )
                    << Profiler::tag_stack.back() << "\t ended   at \t" << stop_time << "\n"
                    << std::endl;
                
                Profiler::id_stack.pop_back();
                Profiler::time_stack.pop_back();
                Profiler::tag_stack.pop_back();
                Profiler::parent_stack.pop_back();
            }
            else
            {
                eprint( "Unmatched ptoc detected. Stack empty. Label =  " + tag + ".");
//                    eprint(std::string("  Expected label =  ") + tag);
//                    eprint(std::string("  Visited label  =  ") + Profiler::tag_stack.back());
            }
        }
        else
        {
            eprint( "Unmatched ptoc detected. Stack empty. Label =  " + tag + ".");
        }
#endif
    }
    
    inline void debug_tic(const std::string & tag)
    {
#ifdef TOOLS_DEBUG
        ptic(tag);
#endif
    }
    
    inline void debug_toc(const std::string & tag)
    {
#ifdef TOOLS_DEBUG
        ptoc(tag);
#endif
    }

    inline void debug_print( std:: string s)
    {
#ifdef TOOLS_DEBUG
        logprint(s);
#endif
    }
    
    inline void debug_assert( bool condition, std:: string s )
    {
#ifdef TOOLS_DEBUG
        if( ! condition )
        {
            eprint(s);
        }
#endif
    }




} // namespace Tools
