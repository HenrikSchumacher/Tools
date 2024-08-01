#pragma once

#include <mutex>

#include <filesystem>

namespace Tools
{
    const std::filesystem::path HomeDirectory()
    {
        const char * s = std::getenv("HOME");

        if( s == nullptr )
        {
            std::filesystem::path home_dir {std::getenv("HOMEDRIVE")};
            
            home_dir /= std::getenv("HOMEPATH");
            
            return home_dir;
        }
        else
        {
            return std::filesystem::path { s };
        }
    }
    
    namespace Profiler
    {
        std::filesystem::path log_file  = HomeDirectory() / "Tools_Log.txt";
        std::filesystem::path prof_file = HomeDirectory() / "Tools_Profile.tsv";
        
        std::mutex log_mutex;
        std::ofstream log  ( log_file );
        
#if defined(TOOLS_ENABLE_PROFILER)
        std::mutex prof_mutex;
        std::ofstream prof ( prof_file );
        
        Time init_time = Clock::now();
        std::vector<Time> time_stack;
        std::vector<std::string> tag_stack(1,"root");
        std::vector<int> parent_stack (1, -0);
        std::vector<int> id_stack (1,0);
        int id_counter = 0;
#endif
        
        
        inline void Clear( const std::filesystem::path & dir, const bool appendQ = false)
        {
            
            const std::lock_guard<std::mutex> log_lock ( log_mutex  );
            
            
            Profiler::log << std::setprecision(16);
            
            Profiler::log_file = dir / "Tools_Log.txt";
            
            Profiler::log.close();

            if( appendQ )
            {
                Profiler::log.open( Profiler::log_file.string(), std::ios_base::app );
            }
            else
            {
                Profiler::log.open( Profiler::log_file.string() );
            }

            print( std::string("Log     will be written to ") + Profiler::log_file.string() + "." );
            Profiler::log << std::setprecision(16);
            
#if defined(TOOLS_ENABLE_PROFILER)
            const std::lock_guard<std::mutex> prof_lock( prof_mutex );
            
            Profiler::log_file = dir / "Tools_Profile.tsv";

            
            print( std::string("Profile will be written to ") + Profiler::log_file.string() + ".");
            
            Profiler::prof.close();
            
            if( appendQ )
            {
                Profiler::prof.open( Profiler::log_file.string(), std::ios_base::app );
            }
            else
            {
                Profiler::prof.open( Profiler::log_file.string() );
            }

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
#endif
        }
        
        inline void Clear()
        {
            Clear( HomeDirectory() );
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
    
    
#define logdump(x) logvalprint( std::string(#x), x );

    
    
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
    
// Print functions that are only active if macro TOOLS_ENABLE_PROFILER is defined.
    
    inline void pprint( const std::string & s )
    {
#ifdef TOOLS_ENABLE_PROFILER
        const std::lock_guard<std::mutex> lock( Profiler::log_mutex );
        Profiler::log << s << "\n" << std::endl;
#else
        (void)s;
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
        
        double start_time = Tools::Duration( Profiler::init_time, Profiler::time_stack.back() );
        
        const std::lock_guard<std::mutex> log_lock( Profiler::log_mutex );
        
        Profiler::log
            << std::string( 2 * (Profiler::time_stack.size()), ' ' )
            << Profiler::tag_stack.back() << "\t started at \t" << start_time << "\n"
            << std::endl;
#else
        (void)tag;
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
                double start_time = Tools::Duration( Profiler::init_time, Profiler::time_stack.back() );
                double stop_time  = Tools::Duration( Profiler::init_time, Clock::now() );
                
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
                eprint( "Unmatched ptoc detected. Tag requested =  " + tag + ". Tag found = " + Profiler::tag_stack.back() + ".");
//                eprint( "Unmatched ptoc detected. Stack empty. Label =  " + tag + ".");
//                    eprint(std::string("  Expected label =  ") + tag);
//                    eprint(std::string("  Visited label  =  ") + Profiler::tag_stack.back());
            }
        }
        else
        {
            eprint( "Unmatched ptoc detected. Stack empty. Label =  " + tag + ".");
        }
#else
        (void)tag;
#endif
    }
    
    inline void debug_tic(const std::string & tag)
    {
#ifdef TOOLS_DEBUG
        ptic(tag);
#else
        (void)tag;
#endif
    }
    
    inline void debug_toc(const std::string & tag)
    {
#ifdef TOOLS_DEBUG
        ptoc(tag);
#else
        (void)tag;
#endif
    }

    inline void debug_print( std:: string s)
    {
#ifdef TOOLS_DEBUG
        logprint(s);
#else
        (void)s;
#endif
    }
    
    inline void debug_assert( bool condition, std:: string s )
    {
#ifdef TOOLS_DEBUG
        if( ! condition )
        {
            eprint(s);
        }
#else
        (void)condition;
        (void)s;
#endif
    }




} // namespace Tools
