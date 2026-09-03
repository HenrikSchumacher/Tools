#pragma once

namespace Tools
{
    class Profiler
    {
    public:
        
        /*!@brief One global instance of the `Logger` to be used by `logprint`, `logvalprint` and other helper functions.
         *
         * We deliberately do not use a singleton here because nonconstant `static` variables behave strangely in shared library environment. This way, each compilation target gets it own copy of `logger`. This is intended, because this code may be used in many small dynamic libraries to be run from one central process. `static` variables would require us to have all these libraries compiled with the same settings `TOOLS_ENABLE_PROFILER`. And this would force us to recompile all libraries if we ever want to use only one of them with profiling information.
         *
         * Note that this design makes it likely that several processes write to the same log file if defaults are used. That is not necessarily a bad thing, but it may ruin the timings recorded in the profile.*/

        Profiler() = delete;
        ~Profiler() = delete;
        
        static Logger<true> & GetLogger()
        {
            static Logger<true> logger { TOOLS_LOG_DIR, TOOLS_LOG_NAME, TOOLS_PROFILE_NAME, true, true };
            return logger;
        }

        /*!@brief Redirected to GetLogger().Clear.*/
        static void Clear(
            const std::filesystem::path & dir,
            const std::string & log_name,
            const std::string & prof_name,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            GetLogger().Clear(dir, log_name, prof_name, silentQ, appendQ );
        }
        
        /*!@brief Redirected to GetLogger().Clear.*/
        static void Clear(
            const std::filesystem::path & dir,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            GetLogger().Clear(dir, TOOLS_LOG_NAME, TOOLS_PROFILE_NAME, silentQ, appendQ );
        }
        
        /*!@brief Redirected to GetLogger().Clear.*/
        static void Clear(
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            GetLogger().Clear(TOOLS_LOG_DIR, TOOLS_LOG_NAME, TOOLS_PROFILE_NAME, silentQ, appendQ );
        }
        
    }; // Profiler

    
    /*!@brief Print message `s` to log file specified by `Tools::GetLogger().LogFile()`.*/
    template<bool printQ = true, bool tabsQ = true, typename ...Args>
    inline void logprint( const Args &... args )
    {
        Profiler::GetLogger().template LogPrint<printQ,tabsQ>(args...);
    }
    
    /*!@brief Print value of `value` associated to tag `tag` to log file specified by `Tools::GetLogger().LogFile()`.*/
    template<bool printQ = true, bool tabsQ = true, typename T>
    static inline void logvalprint( std::string_view tag, const T & value )
    {
        Profiler::GetLogger().template LogValPrint<printQ,tabsQ>(tag,value);
    }
    
#define TOOLS_LOGDUMP(x) Tools::logvalprint( std::string_view(#x), x );
    
#define TOOLS_DDUMP(x) Tools::logvalprint( std::string_view(#x), x ); Tools::valprint( std::string_view(#x), x );
    
    /*!@brief Print WARNING message with text `s`. This is sent to `std::cerr` and to the log file specified by `Tools::logger.LogFile()`.*/
    
    template<typename ...Args>
    inline void wprint( const Args &... args )
    {
        Profiler::GetLogger().WarningPrint(args...);
    }
    
//    static inline void wprint( std::string_view s )
//    {
//        std::string msg ("WARNING: ");
//        msg += s;
//#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
//        print( msg );
//#endif
//        {
//            const std::lock_guard<std::mutex> cerr_lock { Tools::cerr_mutex };
//            std::cerr << msg << std::endl;
//        }
//        logprint<false>(msg);
//    }
    
    /*!@brief Print a NOTE message with text `s`. This is sent to the log file specified by `Tools::GetLogger().LogFile()`.*/
    template<typename ...Args>
    inline void nprint( const Args &... args )
    {
#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
        print("NOTE: ", args...);
#endif
        logprint<false>("NOTE: ", args...);
    }
    
    /*!@brief Print a ERROR message with text `s`. This is sent to `std::cout` and `std::cerr` and to the log file specified by `Tools::GetLogger().LogFile()`.*/
    template<typename ...Args>
    inline void eprint( const Args &... args )
    {
        Profiler::GetLogger().ErrorPrint(args...);
    }
    
    /*!@brief Created an error with `s`. A message is sent to `std::cerr` and to the log file specified by `Tools::GetLogger().LogFile()`. Afterwards, a `std::runtime_error` is throw with this message attached.*/
    template<typename ...Args>
    inline void error( const Args &... args )
    {
        Profiler::GetLogger().Error(args...);
    }
    
//    static inline void error( std::string_view s )
//    {
//        std::string msg ("ERROR: ");
//        msg += s;
//        
//#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
//        print( msg );
//#endif
//        {
//            const std::lock_guard<std::mutex> cerr_lock { Tools::cerr_mutex };
//            std::cerr << msg << std::endl;
//        }
//        logprint<false>( msg );
//        throw std::runtime_error(msg);
//    }
    
   
    /*!@brief Like `logprint`, but only active if macro `TOOLS_ENABLE_PROFILER` is defined.*/
    template<bool tabsQ = true, typename ...Args>
    inline void pprint( const Args &... args )
    {
#ifdef TOOLS_ENABLE_PROFILER
        Profiler::GetLogger().template ProfilePrint<tabsQ>(args...);
#else
        Void{args...};
#endif
    }

/*!@brief Like `logvalprint`, but only active if macro `TOOLS_ENABLE_PROFILER` is defined.*/
    template<typename T>
    static inline void pvalprint( std::string_view tag, const T & value)
    {
#ifdef TOOLS_ENABLE_PROFILER
        Profiler::GetLogger().ProfileValPrint(tag,value);
#else
        Void{tag,value};
#endif
    }

    
#define TOOLS_PDUMP(x) pvalprint( std::string_view(#x), x );

#ifdef TOOLS_ENABLE_PROFILER
    #define TOOLS_PTIC(tag) Tools::Profiler::GetLogger().Tic(tag)
    #define TOOLS_PTOC(tag) Tools::Profiler::GetLogger().Toc(tag)
    #define TOOLS_PTIMER(name, tag) Tools::LoggerTimer name (Tools::Profiler::GetLogger(),tag);
#else
    #define TOOLS_PTIC(tag)
    #define TOOLS_PTOC(tag)
#define TOOLS_PTIMER(name, tag)
#endif
    
    
#ifdef TOOLS_DEBUG
    #define TOOLS_DEBUG_TIC(tag) TOOLS_PTIC(tag);
#else
    #define TOOLS_DEBUG_TIC(tag)
#endif
    
#ifdef TOOLS_DEBUG
    #define TOOLS_DEBUG_TOC(tag) TOOLS_PTOC(tag);
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
