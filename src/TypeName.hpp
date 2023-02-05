#pragma once

#include <stdint.h>

namespace Tools
{
    // Taken from https://stackoverflow.com/a/4485051/8248900
    
    // default implementation
    template<typename T>
    inline constexpr const char * TypeName = typeid(T).name();
    
    template<> inline constexpr const char * TypeName<Int16> = "I16";
    
    template<> inline constexpr const char * TypeName<Int32> = "I32";
    
    template<> inline constexpr const char * TypeName<Int64> = "I64";
    
    template<> inline constexpr const char * TypeName<UInt16> = "U16";
    
    template<> inline constexpr const char * TypeName<UInt32> = "U32";
    
    template<> inline constexpr const char * TypeName<UInt64> = "U64";
    
    template<> inline constexpr const char * TypeName<std::size_t> = "size_t";
    

    
    template<> inline constexpr const char * TypeName<Real32>  = "R32";
    
    template<> inline constexpr const char * TypeName<Real64>  = "R64";
    
    template<> inline constexpr const char * TypeName<Real128> = "R128";

    
    template<> inline constexpr const char * TypeName<Complex32>  = "C32";
    
    template<> inline constexpr const char * TypeName<Complex64>  = "C64";
    
    template<> inline constexpr const char * TypeName<Complex128> = "C16";
    } // namespace Tools
