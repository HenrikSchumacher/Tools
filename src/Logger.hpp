#pragma once

namespace Tools
{
    /*!@brief Class to handle files for logs and profiling with locks. We deliberately do not make this a singleton class because `static` variables may have strange and unpredicted effects in shared library settings. The intended use of this class is to instantiate one instance in `Tools::Profiler` to be used globally. But in principle, it is possible to have several instances of this class to log different things.
     *
     * @tparam profileQ_ Whether also profiles shall be logged. This activates the member function `Logger::Tic(tag)` and `Logger::Toc(tag)` and some other functions related to profiling. Beware that this means typical some considerable overhead, in particular, in parallel sections of code as the locks need to be synchronized. So better use this only in test code and avoid it in production code.
     */
    
    template<bool profileQ_ = false>
    class Logger final
    {
    public:
        
        using Int = std::int64_t;
        static constexpr bool profileQ = profileQ_;
        
        static constexpr bool verboseQ = false; // Only meant for debugging
        
        using Lock_T = std::lock_guard<std::mutex>;
        
        struct StackNode
        {
            Int         id;
            Int         parent;
            std::string tag;
            Time        time;
            
            StackNode(
                Int id_,
                Int parent_,
                const std::string & tag_
            )
            :   id    { id_     }
            ,   parent{ parent_ }
            ,   tag   { tag_    }
            ,   time  { Tools::Clock::now() }
            {}
        };
        
    public:
        
        Logger()
        {
            Clear( HomeDirectory(), "Tools_Log", "Tools_Profile", false, true );
        }
        
        Logger(
            const std::filesystem::path & dir,
            const std::string & log_name,
            const std::string & prof_name,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            Clear( dir, log_name, prof_name, silentQ, appendQ );
        }
        
        ~Logger()
        {
            if constexpr ( verboseQ )
            {
                print("~Logger()");
                print(Info());
            }
        }
        
        // Make this class uncopyable and unmovable.
        Logger(const Logger &) = delete;
        Logger(Logger &&) = delete;
        Logger & operator=(const Logger &) = delete;
        Logger & operator=(Logger &&) = delete;
        
        
        /*!@brief Whether profiling code is active.*/
        static constexpr bool ProfilingQ()
        {
            return profileQ;
        }
        
    private:
        
        std::string construction_date { DateString() };   // DEBUGGING
        std::filesystem::path log_file;
        std::filesystem::path prof_file;
        
        std::mutex    log_mutex;
        std::ofstream log;
        
        std::mutex    prof_mutex;
        std::ofstream prof;
        
        Int id_counter = 1;
        Int blocker_count = 0;
        
        std::vector<StackNode> stack { Size_T{1}, StackNode{ 0, Int(-1), "root" } };
        
    public:
        
        /*!@brief Clear all logging and profiling records and creates new records in the directory `dir`. Logging information will be written to `log_name + ".txt"`; profiling information is sent to `prof_name + ".tsv"`.*/
        void Clear(
            const std::filesystem::path & dir,
            const std::string & log_name,
            const std::string & prof_name,
            const bool silentQ = false,
            const bool appendQ = false
        )
        {
            if constexpr ( verboseQ ) { print(MethodName("Clear") + "(dir,log_name,prof_name,silentQ,appendQ)"); }
            
            const Lock_T log_lock { log_mutex };
            
            construction_date = DateString();
            
            log_file = dir / (log_name + ".txt");
            log.close();
            log.open( log_file, appendQ ? std::ios_base::app : std::ios_base::trunc );
            
            if( log.good() )
            {
                if( !silentQ )
                {
                    print( std::string("Log     will be written to ") + log_file.string() + "." );
                }
                log << std::setprecision(16);
            }
            else
            {
                std::string msg = std::string("ERROR: Logger failed to open file ") + log_file.string() + ".";
                {
                    const Lock_T cerr_lock { Tools::cerr_mutex };
                    std::cerr << msg << std::endl;
                }
                throw std::runtime_error(msg);
            }

            if constexpr ( profileQ )
            {
                const Lock_T prof_lock { prof_mutex };
                prof_file = dir / (prof_name + ".tsv");
                prof.close();
                prof.open( prof_file, appendQ ? std::ios_base::app : std::ios_base::trunc );
                
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
                        const Lock_T cerr_lock { Tools::cerr_mutex };
                        std::cerr << msg << std::endl;
                    }
                    throw std::runtime_error(msg);
                }
                
                blocker_count = 0;
                id_counter = 0;
                stack.clear();
                stack.emplace_back(id_counter++,Int(-1),"root");
            }
            else
            {
                (void)prof_name;
            }
            
            if constexpr ( verboseQ ) { print(Info()); }
        }
        
        /*!@brief Print message `s` to log file.*/
        template<bool tabsQ = true>
        void LogPrint( std::string_view s )
        {
            if constexpr ( verboseQ )
            {
                print(MethodName("LogPrint"));
                print(Info());
            }
            
            const Lock_T log_lock { log_mutex };
            
            if constexpr ( tabsQ && profileQ)
            {
                for( Size_T i = 0; i < stack.size()+1; ++i ) { log << "  "; }
            }
            log << s << "\n" << std::endl;
        }
        
        /*!@brief Print value of `value` associated to tag `tag` to log file.*/
        template<bool tabsQ = true, typename T>
        void LogValPrint( std::string_view tag, const T & value )
        {
            if constexpr ( verboseQ )
            {
                print(MethodName("LogValPrint"));
                print(Info());
            }
            
            const Lock_T log_lock { log_mutex };
            
            if constexpr ( tabsQ && profileQ )
            {
                for( Size_T i = 0; i < stack.size()+1; ++i ) { log <<  "  "; }
            }
            log << tag << " = " << ToString(value) << "\n" << std::endl;
        }
        
        
        /*!@brief Print message `s` to `std::cerr` and to log file.*/
        template<bool tabsQ = true>
        void ErrorPrint( std::string_view s )
        {
            if constexpr ( verboseQ )
            {
                print(MethodName("ErrorPrint"));
                print(Info());
            }
            
            std::string msg ("ERROR: ");
            msg += s;
            
    #if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
            print( msg );
    #endif
            {
                const Lock_T cerr_lock { Tools::cerr_mutex };
                std::cerr << msg << std::endl;
            }
            this->template LogPrint<false>( msg );
        }
        
        
        /*!@brief Print message `s` to log file.*/
        template<bool tabsQ = true>
        void ProfilePrint( std::string_view s )
        {
            if constexpr ( profileQ )
            {
                this->LogPrint<tabsQ>(s);
            }
            else
            {
                (void) s;
            }
        }
        
        /*!@brief Print message `s` to log file.*/
        template<bool tabsQ = true, typename T>
        void ProfileValPrint( std::string_view tag, const T & value )
        {
            if constexpr ( profileQ )
            {
                this->LogValPrint<tabsQ>(tag,value);
            }
            else
            {
                (void)tag;
                (void)value;
            }
        }
        
        /*!@brief Record the start time of an event with tag `tag`.*/
        void Tic( const std::string & tag )
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("Tic") + "(" + tag + ")");
                    print(Info());
                }
                
                if( BlockedQ() ) { return; }
                
                const Lock_T prof_lock { prof_mutex };
                
                stack.emplace_back( id_counter++, stack.back().id, tag );
                
                double start_time = Tools::Duration( stack[0].time, stack.back().time );
                
                const Lock_T log_lock { log_mutex };
                
                for( Size_T i = 0; i < stack.size(); ++i ) { log <<  "  "; }
                
                log << tag << "\t started at \t" << start_time << "\n" << std::endl;
            }
            else
            {
                (void)tag;
            }
        }
        
        /*!@brief Record the end time of an event with tag `tag`.*/
        void Toc( const std::string & tag )
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("Toc") + "(" + tag + ")");
                    print(Info());
                }
                
                if( BlockedQ() ) { return; }
                
                //            print("X");
                const Lock_T prof_lock { prof_mutex };
                //            print("Y");
                
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
                        
                        const Lock_T log_lock { log_mutex };
                        
                        for( Size_T i = 0; i < stack.size(); ++i ) { log << "  "; }
                        
                        log << node.tag << "\t ended   at \t" << stop_time << "\n" << std::endl;
                        
                        stack.pop_back();
                    }
                    else
                    {
                        ErrorPrint( std::string("Unmatched Toc detected. Tag requested = ") + tag + ". Tag found = " + node.tag + ".");
                    }
                }
                else
                {
                    ErrorPrint( std::string("Unmatched Toc detected. Stack empty. Label = ") + tag + ".");
                }
            }
            else
            {
                (void)tag;
            }
        }
        
        void Block()
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("Block"));
                    print(Info());
                }
                
                ++blocker_count;
            }
        }
        
        void ReleaseBlock()
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("ReleaseBlock"));
                    print(Info());
                }
                
                --blocker_count;
            }
        }
        
        bool BlockedQ()
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("BlockedQ"));
                    print(Info());
                }
                
                return (blocker_count > Int(0));
            }
            else
            {
                return false;
            }
        }
        
        Int BlockedCount()
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("BlockedCount"));
                    print(Info());
                }
                
                return blocker_count;
            }
            else
            {
                return 0;
            }
        }
        
    public:

            const std::filesystem::path & LogFile()
            {
                return log_file;
            }
            
            const std::filesystem::path & ProfileFile()
            {
                return prof_file;
            }
            
            std::string Info() const
            {
                std::string s;
                
                s += "Info for instance of ";
                s += ClassName();
                s += "\n";
                
                s += "construction_date = " ;
                s += construction_date;
                s += "\n";
                
                s += "log_file = " ;
                s += log_file.string();
                s += "\n";
                
                s += "prof_file = " ;
                s += prof_file.string();
                s += "\n";
                
                s += "blocker_count = " ;
                s += Tools::ToString(blocker_count);
                s += "\n";
                
                return s;
            }
            
            const StackNode & StackTop() const
            {
                return stack.back();
            }
        
        
    public:
        
        static constexpr std::string MethodName( const std::string & tag )
        {
            return ClassName() + "::" + tag;
        }
        
        static constexpr std::string ClassName()
        {
            return std::string("Logger") + "<" + ToString(profileQ) + ">";
        }
        
    }; // class Logger
    

    
    /*!@brief Declare a local object that prevents profiling calls to a `Logger` during its lifetime. Use this in multi-threaded code passages to prevent misleading info in the profiles (or even file corruption). Constructor and destructor of this class do nothing if  if not set, so the compiler should be able optimize them away.*/
    template<bool profileQ>
    class LoggerBlocker final
    {
    public:
        
        using Logger_T = Logger<profileQ>;
        
    private:
        
        Logger_T & logger;
        
    public:
        
        LoggerBlocker( Logger_T & logger_ )
        :   logger { logger_ }
        {
            if constexpr ( Logger_T::profileQ )
            {
                logger.Block();
            }
        }
        
        ~LoggerBlocker()
        {
            if constexpr ( Logger_T::profileQ )
            {
                logger.ReleaseBlock();
            }
        }
        
        // Make this class uncopyable and unmovable.
        LoggerBlocker(const LoggerBlocker &) = delete;
        LoggerBlocker(LoggerBlocker &&) = delete;
        LoggerBlocker & operator=(const LoggerBlocker &) = delete;
        LoggerBlocker & operator=(LoggerBlocker &&) = delete;
        
    }; // LoggerBlocker
    
    
    /*!@brief Use this to declare a RAII-style timer that records time at construction and destruction and writes according records to the supplied `Logger` instance.*/
    template<bool profileQ>
    class LoggerTimer final
    {
    public:
        
        using Logger_T = Logger<profileQ>;
        
    private:
        
        Logger_T &  logger;
        std::string tag;
        bool        activeQ = false;
        
    public:

        LoggerTimer( Logger_T & logger_, std::string_view tag_ )
        :   logger { logger_ }
        {
            if constexpr ( Logger_T::profileQ )
            {
                tag = tag_;
                activeQ = !logger.BlockedQ();
                if( activeQ ) { logger.Tic(tag); };
            }
            else
            {
                (void)tag;
            }
        }
        
        ~LoggerTimer()
        {
            if constexpr ( Logger_T::profileQ )
            {
                if( activeQ ) { logger.Toc(tag); };
            }
        }
        
        // Make this class uncopyable and unmovable.
        LoggerTimer(const LoggerTimer &) = delete;
        LoggerTimer(LoggerTimer &&) = delete;
        LoggerTimer & operator=(const LoggerTimer &) = delete;
        LoggerTimer & operator=(LoggerTimer &&) = delete;
    
        const std::string & Tag() const
        {
            return tag;
        }
        
        bool ActiveQ() const
        {
            return activeQ;
        }
    }; // LoggerTimer
    
} // namespace Tools
