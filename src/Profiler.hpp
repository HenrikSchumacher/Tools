#pragma once

namespace Tools
{
    namespace Profiler
    {
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
            #pragma omp critical(profiler)
            {
                std::filesystem::path profile_filename = dir;
                profile_filename /= "Tools_Profile.tsv";
                Profiler::prof.close();
                if( append )
                {
                    Profiler::prof.open(profile_filename.c_str(), std::ios_base::app);
                }
                else
                {
                    Profiler::prof.open(profile_filename.c_str());
                }
                std::cout << "Profile will be written to " << profile_filename << "." << std::endl;
                Profiler::log << std::setprecision(16);
                
                std::filesystem::path log_filename = dir;
                log_filename /= "Tools_Log.txt";
                Profiler::log.close();

                if( append )
                {
                    Profiler::log.open(log_filename.c_str(), std::ios_base::app);
                }
                else
                {
                    Profiler::log.open(log_filename.c_str());
                }
                
                std::cout << "Log will be written to " << log_filename << "." << std::endl;
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
        }
        
    }
    
    inline void logprint( const std::string & s )
    {
//        Profiler::log << std::string( 2 * (Profiler::time_stack.size()), ' ');
        #pragma omp critical(profiler)
        {
            Profiler::log << s << "\n" << std::endl;
        }
    }

    template<typename T>
    inline void logvalprint( const std::string & s, const T & value)
    {
        using namespace std;
        logprint( s + " = " + ToString(value) );
    }

    template<typename T>
    inline void logvalprint( const std::string & s, const T & value, const int p)
    {
        logprint( s + " = " + ToString(value, p) );
    }
    
    inline void logvalprint( const std::string & s, const std::string & value)
    {
        using namespace std;
        logprint( s + " = " + value );
    }
    
#define DUMP(x)                                                                 \
{                                                                               \
    _Pragma("omp critical(profiler)")                                           \
    {                                                                           \
        Profiler::log << std::string(#x) << std::string(" = ");                 \
        Profiler::log << x;                                                     \
        Profiler::log << "\n" << std::endl;                                     \
    }                                                                           \
}
    
    inline void eprint( const std::string & s )
    {
        print(    std::string("ERROR: ") + s );
        logprint( std::string("ERROR: ") + s );
    }

    inline void wprint( const std::string & s )
    {
        print(    std::string("WARNING: ") + s );
        logprint( std::string("WARNING: ") + s );
    }
    
    inline void ptic_(const std::string & tag)
    {
        #pragma omp critical(profiler)
        {
            Profiler::time_stack.push_back(Clock::now());
            Profiler::parent_stack.push_back(Profiler::id_stack.back());
            Profiler::tag_stack.push_back(tag);
            Profiler::id_stack.push_back(++Profiler::id_counter);
            
            float start_time = Duration( Profiler::init_time, Profiler::time_stack.back() );
            
            for( std::size_t i = 0; i < Profiler::time_stack.size(); ++i )
            {
                Profiler::log << "  ";
            }
            Profiler::log
                << Profiler::tag_stack.back() << "\t started at \t" << start_time << "\n"
                << std::endl;
        }
    }
    
    inline void ptoc_(const std::string & tag)
    {
        #pragma omp critical(profiler)
        {
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
                    
                    for( std::size_t i = 0; i < Profiler::time_stack.size(); ++i )
                    {
                        Profiler::log << "  ";
                    }
                    Profiler::log
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
        }
    }
    
#ifdef TOOLS_ENABLE_PROFILER
    #define ptic(tag) ptic_(tag)
    #define ptoc(tag) ptoc_(tag)
#else
    #define ptic(tag)
    #define ptoc(tag)
#endif
    
    
#define pdump(x)                                                                \
{                                                                               \
    _Pragma("omp critical(profiler)")                                           \
    {                                                                           \
        Profiler::log << std::string(#x) << std::string(" = ");                 \
        Profiler::log << x;                                                     \
        Profiler::log << "\n" << std::endl;                                     \
    }                                                                           \
}

}
