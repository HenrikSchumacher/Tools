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
        
        std::string construction_date { DateString() };
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
            if constexpr ( verboseQ )
            {
                print(MethodName("Clear(dir,log_name,prof_name,silentQ,appendQ)"));
            }
            
            const Lock_T log_lock { log_mutex };
            
            construction_date = DateString();
            
            log_file = dir / (log_name + ".txt");
            log.close();
            log.open( log_file, appendQ ? std::ios_base::app : std::ios_base::trunc );
            
            if( log.good() )
            {
                if( !silentQ )
                {
                    print("Log     will be written to ", log_file.string(), "." );
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
                        print("Profile will be written to ", prof_file.string(), ".");
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
        
    private:
        
        /*!@brief Print message `s` to log file.*/
        void LogPrint_Raw( std::string_view s )
        {
            if constexpr ( verboseQ )
            {
                print(MethodName("LogPrint_Raw"));
                print(Info());
            }
            
            const Lock_T log_lock { log_mutex };
            log << s << std::endl;
        }
        
    public:
        
//        template<bool tabsQ = true, typename A>
//        void LogPrint( A && a )
//        {
//            // We forward the first argument's type because it could be an OutString &&
//            if constexpr ( tabsQ )
//            {
//                LogPrint_Raw<tabsQ>(OutString::FromMisc(
//                    OutString(' ', 2 * stack.size() + 1),
//                    std::forward<A>(a)
//                ).View());
//            }
//            else
//            {
//                LogPrint_Raw<tabsQ>(OutString::FromMisc(
//                    std::forward<A>(a)
//                ).View());
//            }
//        }
        
        /*!@brief Print items to log file, using convertion by `OutString`.*/
        template<bool printQ = true, bool tabsQ = true, typename A, typename ...Args>
        inline void LogPrint( const A & a, const Args &... args )
        {
            if constexpr ( printQ )
            {
                if constexpr ( tabsQ )
                {
                    // We forward the first argument's type because it could be an OutString &&
                    LogPrint_Raw(OutString::FromMisc(
                        OutString(Size_T{2}*stack.size()+Size_T{1},' '),
                        a,
                        args...
                    ).View());
                }
                else
                {
                    // We forward the first argument's type because it could be an OutString &&
                    LogPrint_Raw(OutString::FromMisc(
                        a,
                        args...
                    ).View());
                }
            }
            else
            {
                Void{a,args...};
            }
        }

        /*!@brief Print value of `value` associated to tag `tag` to log file.*/
        template<bool printQ = true, bool tabsQ = true>
        void LogValPrint( std::string_view tag, std::string_view value )
        {
            LogPrint<printQ,tabsQ>( tag, " = ", value );
        }
        
        /*!@brief Print value of `value` associated to tag `tag` to log file.*/
        template<bool printQ = true, bool tabsQ = true, typename T>
        void LogValPrint( std::string_view tag, const T & value )
        {
            if constexpr ( printQ )
            {
                LogPrint<printQ,tabsQ>( tag, " = ", ToString(value) );
            }
            else
            {
                Void{tag,value};
            }
        }
        
    private:
        
        /*!@brief Print message `s` to `std::cerr` and to log file.*/
        template<bool throwQ = false>
        void ErrorPrint_Raw( std::string_view s )
        {
            if constexpr ( verboseQ )
            {
                print(MethodName("ErrorPrint_Raw"));
                print(Info());
            }
            
    #if defined(LTEMPLATE_H) || defined(TENSORS_MMA_HPP)
            print(s);
    #endif
            {
                const Lock_T cerr_lock { Tools::cerr_mutex };
                std::cerr << s << std::endl;
            }
            this->template LogPrint<true,false>(s);
            
            if constexpr ( throwQ )
            {
                throw std::runtime_error(std::string(s));
            }
        }
        
        template<bool throwQ = false, typename ...Args>
        void ErrorPrint_Raw( const Args &... args )
        {
            ErrorPrint_Raw<throwQ>(OutString::FromMisc(args...).View());
        }
        
    public:
        
        /*!@brief Print arguments to as ERROR `std::cerr` and to log file.*/
        template<typename ...Args>
        void ErrorPrint( const Args &... args )
        {
            ErrorPrint_Raw("ERROR: ", args...);
        }
        
        /*!@brief Print message `s` as WARNING to `std::cerr` and to log file.*/
        template<typename ...Args>
        void WarningPrint( const Args &... args )
        {
            ErrorPrint_Raw("WARNING: ", args...);
        }
        
        /*!@brief Print arguments to as ERROR `std::cerr` and to log file. Then throw an exception.*/
        template<typename ...Args>
        void Error( const Args &... args )
        {
            ErrorPrint_Raw<true>("ERROR: ", args...);
        }
        
        /*!@brief Print message `s` to log file.*/
        template<bool tabsQ = true, typename ...Args>
        void ProfilePrint( const Args &... args )
        {
            if constexpr ( profileQ )
            {
                this->LogPrint<tabsQ>(args...);
            }
            else
            {
                Void{args...};
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
                Void{tag,value};
            }
        }
        
        /*!@brief Record the start time of an event with tag `tag`.*/
        void Tic( std::string_view tag )
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("Tic"), "(", tag, ")");
                    print(Info());
                }
                
                if( BlockedQ() ) { return; }
                
                const Lock_T prof_lock { prof_mutex };
                
                stack.emplace_back( id_counter++, stack.back().id, std::string(tag) );
                
                double start_time = Tools::Duration( stack[0].time, stack.back().time );
                
                const Lock_T log_lock { log_mutex };
                
                for( Size_T i = 0; i < stack.size(); ++i ) { log <<  "  "; }
                
                log << tag << "\t started at \t" << start_time << "\n" << std::endl;
            }
            else
            {
                Void{tag};
            }
        }
        
        /*!@brief Record the end time of an event with tag `tag`.*/
        void Toc( std::string_view tag )
        {
            if constexpr ( profileQ )
            {
                if constexpr ( verboseQ )
                {
                    print(MethodName("Toc"), "(", tag, ")");
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
                        ErrorPrint("Unmatched Toc detected. Tag requested = ", tag, ". Tag found = ", node.tag, ".");
                    }
                }
                else
                {
                    ErrorPrint("Unmatched Toc detected. Stack empty. Label = ", tag, ".");
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
            
            OutString Info() const
            {
                return OutString::FromMisc(
                    "Info for instance of ", ClassName(), "\n",
                    "construction_date = ", construction_date, "\n",
                    "log_file = ", log_file.string(), "\n",
                    "prof_file = ", prof_file.string(), "\n",
                    "blocker_count = ", blocker_count, "\n"
                );
            }
            
            const StackNode & StackTop() const
            {
                return stack.back();
            }
        
    public:
        
        template<Size_T N>
        static consteval auto MethodName( const ct_string<N> & tag )
        {
            return ClassName() + "::" + tag;
        }
        
        template<Size_T N>
        static consteval auto MethodName( const char(&tag)[N] )
        {
            return MethodName( ct_string(tag) );
        }
        
        static consteval auto ClassName()
        {
            return ct_string("Logger<") + to_ct_string(profileQ) + ">";
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
