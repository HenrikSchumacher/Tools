#pragma once

#include <mutex>

#include <filesystem>

namespace Tools
{
    
    /*!
     * @brief Return user's home directory.
     */
    
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
            
            Profiler::prof_file = dir / "Tools_Profile.tsv";

            print( std::string("Profile will be written to ") + Profiler::prof_file.string() + ".");
            
            Profiler::prof.close();
            
            if( appendQ )
            {
                Profiler::prof.open( Profiler::prof_file.string(), std::ios_base::app );
            }
            else
            {
                Profiler::prof.open( Profiler::prof_file.string() );
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
    
    /*!
     * @brief Print to log file specified in `Profiler::log_file`.
     */
    
    inline void logprint( const std::string & s )
    {
        const std::lock_guard<std::mutex> lock( Profiler::log_mutex );
        Profiler::log << s << "\n" << std::endl;
    }
    
    template<Size_T N>
    inline void logprint( const ct_string<N> & s )
    {
        logprint( s.data() );
    }

    /*!
     * @brief Print to log file specified in `Profiler::log_file`.
     */
    
    template<typename T>
    inline void logvalprint( const std::string & s, const T & value)
    {
        logprint( s + " = " + ToString(value) );
    }
    
    template<Size_T N, typename T>
    inline void logvalprint( const ct_string<N> & s, const T & value)
    {
        logvalprint( s.data(), value );
    }

    /*!
     * @brief Print to log file specified in `Profiler::log_file`.
     */
    
    template<typename T>
    inline void logvalprint( const std::string & s, const T & value, const int p)
    {
        logprint( s + " = " + ToString(value, p) );
    }
    
    template<Size_T N, typename T>
    inline void logvalprint( const ct_string<N> & s, const T & value, const int p)
    {
        logvalprint( s.data(), value, p  );
    }
    
    /*!
     * @brief Print to log file specified in `Profiler::log_file`.
     */
    
    inline void logvalprint( const std::string & s, const std::string & value)
    {
        logprint( s + " = " + value );
    }
    
    template<Size_T N>
    inline void logvalprint( const ct_string<N> & s, const std::string & value)
    {
        logvalprint( s.data(), value  );
    }
    
    
#define logdump(x) logvalprint( std::string(#x), x );

    
    /*!
     * @brief Print an error to `std::cout`.
     */
    
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
    
    template<Size_T N>
    inline void eprint( const ct_string<N> & s )
    {
        eprint( s.data() );
    }

    
    /*!
     * @brief Print a warning to `std::cout`.
     */
    
    inline void wprint( const std::string & s )
    {
        print(    std::string("WARNING: ") + s );
        logprint( std::string("WARNING: ") + s );
    }
    
    template<Size_T N>
    inline void wprint( const ct_string<N> & s )
    {
        wprint(s.data());
    }
    
    /*!
     * @brief This print function is only active if macro `TOOLS_ENABLE_PROFILER` is defined.
     * It prints to the log file specified in `Profiler::log_file`.
     */
    
    inline void pprint( const std::string & s )
    {
#ifdef TOOLS_ENABLE_PROFILER
        const std::lock_guard<std::mutex> lock( Profiler::log_mutex );
        Profiler::log << s << "\n" << std::endl;
#else
        (void)s;
#endif
    }

    /*!
     * @brief This print function is only active if macro `TOOLS_ENABLE_PROFILER` is defined.
     * It prints to the log file specified in `Profiler::log_file`.
     */
    
    template<typename T>
    inline void pvalprint( const std::string & s, const T & value)
    {
#ifdef TOOLS_ENABLE_PROFILER
        pprint( s + " = " + ToString(value) );
#else
        (void)s;
        (void)value;
#endif
    }

    /*!
     * @brief This print function is only active if macro `TOOLS_ENABLE_PROFILER` is defined.
     * It prints to the log file specified in `Profiler::log_file`.
     */
    
    template<typename T>
    inline void pvalprint( const std::string & s, const T & value, const int p)
    {
#ifdef TOOLS_ENABLE_PROFILER
        pprint( s + " = " + ToString(value, p) );
#else
        (void)s;
        (void)value;
        (void)p;
#endif
    }
    
    /*!
     * @brief This print function is only active if macro `TOOLS_ENABLE_PROFILER` is defined.
     * It prints to the log file specified in `Profiler::log_file`.
     */
    
    inline void pvalprint( const std::string & s, const std::string & value)
    {
#ifdef TOOLS_ENABLE_PROFILER
        pprint( s + " = " + value );
#else
        (void)s;
        (void)value;
#endif
    }
    
#define pdump(x) pvalprint( std::string(#x), x );

    
#ifdef TOOLS_ENABLE_PROFILER
    #define ptic( s ) ptic_impl(s)
#else
    #define ptic( s )
#endif
    
#ifdef TOOLS_ENABLE_PROFILER
    #define ptoc( s ) ptoc_impl(s)
#else
    #define ptoc( s )
#endif
    
    inline void ptic_impl(const std::string & tag)
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
    
    template<Size_T N>
    inline void ptic_impl( const ct_string<N> & tag )
    {
#ifdef TOOLS_ENABLE_PROFILER
        ptic(tag.data());
#else
        (void)tag;
#endif
    }
    
    inline void ptoc_impl(const std::string & tag)
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
    
    template<Size_T N>
    inline void ptoc_impl( const ct_string<N> & tag )
    {
#ifdef TOOLS_ENABLE_PROFILER
        ptoc(tag.data());
#else
        (void)tag;
#endif
    }
    
    
#ifdef TOOLS_DEBUG
    #define TOOLS_DEBUG_TIC(tag) ptic(tag);
#else
    #define TOOLS_DEBUG_TIC(tag)
#endif
    
#ifdef TOOLS_DEBUG
    #define TOOLS_DEBUG_TOC(tag) ptoc(tag);
#else
    #define TOOLS_DEBUG_TOC(tag)
#endif
    
#ifdef TOOLS_DEBUG
    #define TOOLS_DEBUG_PRINT(s) logprint(s);
#else
    #define TOOLS_DEBUG_PRINT(s)
#endif
    
#ifdef TOOLS_DEBUG
    #define TOOLS_DEBUG_ASSERT(condition, s) if( ! condition ) { eprint(s); }
#else
    #define TOOLS_DEBUG_ASSERT(condition, s)
#endif

} // namespace Tools
