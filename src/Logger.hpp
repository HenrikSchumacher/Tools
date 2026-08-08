#pragma once

#include <mutex>
#include <filesystem>

namespace Tools
{
    /*!@brief Return user's home directory.*/
    static std::filesystem::path HomeDirectory(void)
    {
        // Should work on all POSIX systems.
        {
            const char * s = std::getenv("HOME");
            
            if( s != nullptr ) { return std::filesystem::path { s }; }
        }
        
        {
            const char * s = std::getenv("USERPROFILE");
            
            if( s != nullptr ) { return std::filesystem::path { s }; }
        }
        
        return std::filesystem::temp_directory_path();
    }
    
    class Profiler;
    
    /*!@brief Singleton class to handle files for logs and profiling. */
    class Logger
    {
    private:
        
        Logger() {}
        
    public:
        
        Logger(const Logger &) = delete;
        Logger & operator=(const Logger &) = delete;
        
        friend class Profiler;
        //        friend class Profiler::Blocker;
        //        friend class Profiler::Timer;
        
    private:
        
        using Int = std::int64_t;
        
        std::filesystem::path log_file;
        std::filesystem::path prof_file;
        
        std::mutex log_mutex;
        std::ofstream log;
        
        // Only used in builds with TOOLS_ENABLE_PROFILER.
        [[maybe_unused]] int blocker_count = 0;
        
        struct StackNode
        {
            Int id;
            Int parent;
            std::string tag;
            Time time;
            
            StackNode(
                      Int id_,
                      Int parent_,
                      const std::string & tag_
                      )
            :   id    ( id_     )
            ,   parent( parent_ )
            ,   tag   ( tag_    )
            ,   time  ( Tools::Clock::now() )
            {}
        };
        
#if defined(TOOLS_ENABLE_PROFILER)
        std::mutex prof_mutex;
        std::ofstream prof;
        
        Int id_counter = 1;
        
        std::vector<StackNode> stack {
            Size_T{1}, StackNode{ 0, Int(-1), "root" }
        };
        
#endif // defined(TOOLS_ENABLE_PROFILER)
        
    private:
        
        /*!@brief Return instance without initialization.*/
        static Logger & Get()
        {
            static Logger profiler;
            return profiler;
        }
        
        /*!@brief Return instance; make sure that it is initialized.*/
        static Logger & GetInstance()
        {
            Logger & profiler = Logger::Get();
            
            if( !profiler.log.good() )
            {
                profiler.Clear_Private(false,false);
            }
            
            return profiler;
        }
        
        void Clear_Private(
            const std::filesystem::path & dir,
            const std::string & log_name,
            const std::string & prof_name,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            const std::lock_guard<std::mutex> log_lock ( log_mutex  );
            
            log_file = dir / (log_name + ".txt");
            log.close();
            log.open( log_file, appendQ ? std::ios_base::app : std::ios_base::trunc );
            
            if( log.good() )
            {
                if( !silentQ )
                {
                    print( std::string("Log     will be written to ") + log_file.string() + "." );
                }
            }
            else
            {
                std::string msg = std::string("ERROR: Logger failed to open file ") + log_file.string() + ".";
                {
                    const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
                    std::cerr << msg << std::endl;
                }
                throw std::runtime_error(msg);
            }
            
            log << std::setprecision(16);
            
#ifdef TOOLS_ENABLE_PROFILER
            const std::lock_guard<std::mutex> prof_lock( prof_mutex );
            
            prof_file = dir / (prof_name + ".tsv");
            
            prof.close();
            
            if( appendQ )
            {
                prof.open( prof_file, std::ios_base::app );
            }
            else
            {
                prof.open( prof_file );
            }
            
            if( prof.good() )
            {
                if( !silentQ )
                {
                    print( std::string("Profile will be written to ") + prof_file.string() + ".");
                }
            }
            else
            {
                std::string msg = std::string("ERROR: Logger failed to open file ") + prof_file.string() + "." ;
                {
                    const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
                    std::cerr << msg << std::endl;
                }
                throw std::runtime_error(msg);
            }
            
            blocker_count = 0;
            id_counter = 0;
            stack.clear();
            stack.emplace_back(id_counter++,Int(-1),"root");
#else
            (void)prof_name;
#endif // TOOLS_ENABLE_PROFILER
        }
        
        void Clear_Private(
            const std::filesystem::path & dir,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            
            std::string log_name;
#ifdef TOOLS_LOG_NAME
            log_name = std::filesystem::path(TOOLS_PROFILE_NAME);
#else
            log_name = "Tools_Log";
#endif
            
            std::string prof_name;
#ifdef TOOLS_PROFILE_NAME
            prof_name = std::filesystem::path(TOOLS_PROFILE_NAME);
#else
            prof_name = "Tools_Profile";
#endif
            Clear_Private( dir, log_name, prof_name, silentQ, appendQ );
        }
        
        void Clear_Private( const bool silentQ = false, const bool appendQ = false )
        {
            std::filesystem::path dir;
            std::string log_name;
            std::string prof_name;
            
#ifdef TOOLS_LOG_DIR
            dir = std::filesystem::path(TOOLS_LOG_DIR);
#else
            dir = HomeDirectory();
#endif
            Clear_Private( dir, silentQ, appendQ );
        }
        
        template<bool tabsQ = true>
        inline void LogPrint_Private( std::string_view s )
        {
            const std::lock_guard<std::mutex> lock( log_mutex );
            if constexpr ( tabsQ )
            {
#if defined(TOOLS_ENABLE_PROFILER)
                for( Size_T i = 0; i < stack.size()+1; ++i ) { log << "  "; }
#endif // defined(TOOLS_ENABLE_PROFILER)
            }
            log << s << "\n" << std::endl;
        }
        
        template<bool tabsQ = true, typename T>
        inline void LogValPrint_Private( std::string_view tag, const T & value )
        {
            const std::lock_guard<std::mutex> lock( log_mutex );
            if constexpr ( tabsQ )
            {
#if defined(TOOLS_ENABLE_PROFILER)
                for( Size_T i = 0; i < stack.size()+1; ++i ) { log <<  "  "; }
#endif // defined(TOOLS_ENABLE_PROFILER)
            }
            log << tag << " = " << ToString(value) << "\n" << std::endl;
        }
        
        inline void Tic_Private( const std::string & tag )
        {
#ifdef TOOLS_ENABLE_PROFILER
            
            if( blocker_count > 0 ) { return; }
            
            const std::lock_guard<std::mutex> prof_lock( prof_mutex );
            
            stack.emplace_back( id_counter++, stack.back().id, tag );
            
            double start_time = Tools::Duration( stack[0].time, stack.back().time );
            
            const std::lock_guard<std::mutex> log_lock( log_mutex );
            
            for( Size_T i = 0; i < stack.size(); ++i ) { log <<  "  "; }
            
            log << tag << "\t started at \t" << start_time << "\n" << std::endl;
#else
            (void)tag;
#endif
        }
        
        inline void Toc_Private( const std::string & tag )
        {
#ifdef TOOLS_ENABLE_PROFILER
            
            if( blocker_count > 0 ) { return; }
            
            const std::lock_guard<std::mutex> prof_lock( prof_mutex );
            
            if( !stack.empty() )
            {
                StackNode & node = stack.back();
                
                if( tag == node.tag )
                {
                    Time & init_time  = stack[0].time;
                    double start_time = Tools::Duration( init_time, node.time );
                    double stop_time  = Tools::Duration( init_time, Clock::now() );
                    
                    prof
                    << node.id <<  "\t"
                    << node.tag << "\t"
                    << node.parent << "\t"
                    << start_time << "\t"
                    << stop_time << "\t"
                    << stop_time-start_time << "\t"
                    << stack.size()-1
                    << std::endl;
                    
                    const std::lock_guard<std::mutex> log_lock( log_mutex );
                    
                    for( Size_T i = 0; i < stack.size(); ++i ) { log << "  "; }
                    
                    log << node.tag << "\t ended   at \t" << stop_time << "\n" << std::endl;
                    
                    stack.pop_back();
                }
                else
                {
                    ErrorPrint( std::string("Unmatched Logger::Toc detected. Tag requested = ") + tag + ". Tag found = " + node.tag + ".");
                }
            }
            else
            {
                ErrorPrint( std::string("Unmatched Logger::Toc detected. Stack empty. Label = ") + tag + ".");
            }
#else
            (void)tag;
#endif
        }
        
    private:
        
        static void Block()
        {
            ++Get().blocker_count;
        }
        
        static void ReleaseBlock()
        {
            --Get().blocker_count;
        }
        
        static bool BlockedQ()
        {
            return (Get().blocker_count > 0);
        }
        
    public:
        
        static int BlockedCount()
        {
            return Get().blocker_count;
        }
        
    public:
        
        /*!@brief Clear all logging and profiling records and creates new records in the directory `dir`. Logging information will be written to `log_name + ".txt"`; profiling information is sent to `prof_name + ".tsv".*/
        static void Clear(
                          const std::filesystem::path & dir,
                          const std::string & log_name,
                          const std::string & prof_name,
                          const bool silentQ = false,
                          const bool appendQ = false
                          )
        {
            Logger::Get().Clear_Private(dir,log_name,prof_name,silentQ,appendQ);
        }
        
        /*!@brief Clear all logging and profiling records and create new records in the directory `dir`. If the preprocessor macro `TOOLS_LOG_NAME` is set, then the logs will be recorded in the file `TOOLS_LOG_NAME + ".txt"`; otherwise in `dir / "Tools_Log.txt"`. Likewise, if `TOOLS_PROFILE_NAME` is set, then the profiling information will be written to `TOOLS_PROFILE_NAME + ".tsv"`; otherwise `"Tools_Profile.tsv"` will be used.*/
        static void Clear(
                          const std::filesystem::path & dir,
                          const bool silentQ = false,
                          const bool appendQ = false
                          )
        {
            Logger::Get().Clear_Private(dir,silentQ,appendQ);
        }
        
        /*!@brief Clear all logging and profiling records and creates new records.
         If `TOOLS_LOG_DIR` is set, then logging and profiling information will be recorded in the directory `TOOLS_LOG_DIR` in the files  `"Tools_Log.txt"` and  `"Tools_Profile.tsv"`, respectively.*/
        static inline void Clear(
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            Logger::Get().Clear_Private(silentQ,appendQ);
        }
        
        
        /*!@brief Record the start time of an event with tag `tag`.*/
        static inline void Tic( const std::string & tag )
        {
#ifdef TOOLS_ENABLE_PROFILER
            Logger::GetInstance().Tic_Private(tag);
#else
            (void)tag;
#endif
        }
        
        /*!@brief Record the end time of an event with tag `tag`.*/
        static inline void Toc( const std::string & tag )
        {
#ifdef TOOLS_ENABLE_PROFILER
            Logger::GetInstance().Toc_Private(tag);
#else
            (void)tag;
#endif
        }
        
        
        /*!@brief Print message `s` to log file specified in `Logger::File()`.*/
        template<bool tabsQ = true>
        static inline void LogPrint( std::string_view s )
        {
            Logger::GetInstance().template LogPrint_Private<tabsQ>(s);
        }
        
        /*!@brief Print value of `value` associated to tag `tag` to file specified by `Logger::File()`.*/
        template<bool tabsQ = true, typename T>
        static inline void LogValPrint( std::string_view tag, const T & value )
        {
            Logger::GetInstance().template LogValPrint_Private<tabsQ>(tag,value);
        }
        
        /*!@brief Print message `s` to `std::cerr` and to log file specified in `Logger::File()`.*/
        template<bool tabsQ = true>
        static inline void ErrorPrint( std::string_view s )
        {
            std::string msg ("ERROR: ");
            msg += s;
            
    #if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
            print( msg );
    #endif
            {
                const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
                std::cerr << msg << std::endl;
            }
            Logger:: template LogPrint<false>( msg );
        }
        
        /*!@brief Return the path of the currently used log file.*/
        static cref<std::filesystem::path> File()
        {
            return Logger::GetInstance().log_file;
        }
        
        /*!@brief Whether code was compiled with prepreocessor macro `TOOLS_ENABLE_PROFILER`.*/
        static constexpr bool ProfilingQ()
        {
#ifdef TOOLS_ENABLE_PROFILER
            return true;
#else
            return false;
#endif
        }
        
    }; // class Logger
    
    
    /*!@brief A slim wrapper class around `Logger` to handle profiling. */
    class Profiler
    {
        
    public:
        
        /*!@brief Declare a local object that prevents profiling calls during its lifetime. Use this in multi-threaded code passages to prevent misleading info in the profiles (or even file corruption). Constructor and destructor of this class do absolutely nothing if `TOOLS_ENABLE_PROFILER` if not set, so the compiler should be able optimize them away.*/
        class Blocker
        {
        public:
            
            Blocker()
            {
#if defined(TOOLS_ENABLE_PROFILER)
                Logger::Block();
#endif
            }
            
            ~Blocker()
            {
#if defined(TOOLS_ENABLE_PROFILER)
                Logger::ReleaseBlock();
#endif
            }
            
        }; // Blocker
        
        /*!@brief Use this to declare a RAII-style timer that writes records time at construction and destruction and writes according records to `Logger::ProfilerFile()`. This class is declared only if `TOOLS_ENABLE_PROFILER` is set. Use the macro `TOOLS_PTIMER(name,tag)` instead of `Timer name(tag)` to get clean no-ops if `TOOLS_ENABLE_PROFILER` is not set. (Not creating this class if `TOOLS_ENABLE_PROFILER` is not set is deliberate.)*/
        class Timer final
        {
        private:
            
            const std::string tag;
            bool activeQ = false;
            
        public:
            
            const std::string & Tag() const
            {
                return tag;
            }
            
            bool ActiveQ() const
            {
                return activeQ;
            }
            
#ifdef TOOLS_ENABLE_PROFILER
            Timer( std::string_view tag_ )
            :   tag { tag_ }
            ,   activeQ( !Logger::BlockedQ() )
            {
                if( activeQ ) { Logger::Tic(tag); };
            }
            
            ~Timer()
            {
                if( activeQ ) { Logger::Toc(tag); };
            }
#else
            Timer( std::string_view tag_ )
            {
                (void)tag_;
            }
            
            ~Timer() = default;
#endif
        }; // Timer
        
        
        /*!@brief Return the path of the currently used profiling file.*/
        static const std::filesystem::path & File()
        {
            return Logger::GetInstance().prof_file;
        }
        
        // Backward compatibility; redirected to Logger::Clear.
        static void Clear(
            const std::filesystem::path & dir,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            Logger::Clear(dir, silentQ, appendQ );
        }
        
        // Backward compatibility; redirected to Logger::Clear.
        static void Clear(
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            Logger::Clear(silentQ, appendQ );
        }
        
    }; // Profiler

    
    /*!@brief Print message `s` to log file specified in `Logger::File()`.*/
    template<bool tabsQ = true>
    inline void logprint( std::string_view s )
    {
        Logger::template LogPrint<tabsQ>(s);
    }
    
    /*!@brief Print value of `value` associated to tag `tag` to file specified by `Logger::File()`.*/
    template<bool tabsQ = true, typename T>
    inline void logvalprint( std::string_view tag, const T & value )
    {
        Logger::template LogValPrint<tabsQ>(tag,value);
    }
    
    
#define TOOLS_LOGDUMP(x) Tools::logvalprint( std::string_view(#x), x );
    
#define TOOLS_DDUMP(x) Tools::logvalprint( std::string_view(#x), x ); Tools::valprint( std::string_view(#x), x );
    
    /*!@brief Print WARNING message with text `s`. This is sent to `std::cerr` and to the log file specified in `Logger::File()`.*/
    inline void wprint( std::string_view s )
    {
        std::string msg ("WARNING: ");
        msg += s;
#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
        print( msg );
#endif
        {
            const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
            std::cerr << msg << std::endl;
        }
        logprint<false>(msg);
    }
    
    /*!@brief Print a NOTE message with text `s`. This is sent to the log file specified in `Logger::File()`.*/
    inline void nprint( std::string_view s )
    {
        std::string msg ("NOTE: ");
        msg += s;
#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
        print( msg );
#endif
        logprint<false>(msg);
    }
    
    /*!@brief Print a ERROR message with text `s`. This is sent to `std::cout` and `std::cerr` and to the log file specified in `Logger::File()`.*/
    inline void eprint( std::string_view s )
    {
        Logger::ErrorPrint(s);
    }
    
    /*!@brief Created an error with `s`. A message is sent to `std::cerr` and to the log file specified in `Logger::File()`. Afterwards, a `std::runtime_error` is throw with this message attached.*/
    inline void error( std::string_view s )
    {
        std::string msg ("ERROR: ");
        msg += s;
        
#if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
        print( msg );
#endif
        {
            const std::lock_guard<std::mutex> cerr_lock( Tools::cerr_mutex );
            std::cerr << msg << std::endl;
        }
        logprint<false>( msg );
        throw std::runtime_error(msg);
    }
    
   
    /*!@brief Like `logprint`, but only active if macro `TOOLS_ENABLE_PROFILER` is defined.*/
    
    inline void pprint( std::string_view s )
    {
#ifdef TOOLS_ENABLE_PROFILER
        logprint(s);
#else
        (void)s;
#endif
    }

/*!@brief Like `logvalprint`, but only active if macro `TOOLS_ENABLE_PROFILER` is defined.*/
    
    template<typename T>
    inline void pvalprint( std::string_view s, const T & value)
    {
#ifdef TOOLS_ENABLE_PROFILER
        logvalprint(s,value);
#else
        (void)s;
        (void)value;
#endif
    }

    
#define TOOLS_PDUMP(x) pvalprint( std::string_view(#x), x );

    
#ifdef TOOLS_ENABLE_PROFILER
    #define TOOLS_PTIC(s) Tools::Logger::Tic(s)
    #define TOOLS_PTOC(s) Tools::Logger::Toc(s)
    #define TOOLS_PTIMER(name, s) Tools::Profiler::Timer name (s);
#else
    #define TOOLS_PTIC(s)
    #define TOOLS_PTOC(s)
#define TOOLS_PTIMER(name, s)
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
