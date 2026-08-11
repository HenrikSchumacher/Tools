#pragma once

#include <filesystem>

namespace Tools
{
    /*!@brief Return a string with the current date and time.*/
    static inline std::string DateString(void)
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        return s.str();
    }
    
    /*!@brief Return user's home directory.*/
    static inline std::filesystem::path HomeDirectory(void)
    {
        // Should work on all POSIX systems.
        {
            const char * s = std::getenv("HOME");
            
            if( s != nullptr ) { return std::filesystem::path { s }; }
        }
        
        // Should be the home directory on Windows.
        {
            const char * s = std::getenv("USERPROFILE");
            
            if( s != nullptr ) { return std::filesystem::path { s }; }
        }
        
        return std::filesystem::temp_directory_path();
    }
    
    
#ifndef TOOLS_LOG_DIR
    #define TOOLS_LOG_DIR Tools::HomeDirectory()
#endif
        
#ifndef TOOLS_LOG_NAME
    #define TOOLS_LOG_NAME "Tools_Log"
#endif
        
#ifndef TOOLS_PROFILE_NAME
    #define TOOLS_PROFILE_NAME "Tools_Profile"
#endif

#ifdef TOOLS_ENABLE_PROFILER
    #define TOOLS_PROFILEQ true
#else
    #define TOOLS_PROFILEQ false
#endif
    
} // namespace Tools
