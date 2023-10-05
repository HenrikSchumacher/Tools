#pragma once

#include <stdint.h>

namespace Tools
{
    // Taken from https://stackoverflow.com/a/4485051/8248900
    
    // default implementation
    template<typename T>
    constexpr const char * TypeName = "UnknownType";
    
    template<> constexpr const char * TypeName<bool> = "Bool";
    
    template<> constexpr const char * TypeName<Int16> = "I16";
    
    template<> constexpr const char * TypeName<Int32> = "I32";
    
    template<> constexpr const char * TypeName<Int64> = "I64";
    
    template<> constexpr const char * TypeName<UInt16> = "U16";
    
    template<> constexpr const char * TypeName<UInt32> = "U32";
    
    template<> constexpr const char * TypeName<UInt64> = "U64";
    
    
    // Apparently, GCC treats `long` as an alias (e.g. for `long long` on 64-bit systems).
    // clang treats the types `long` and `long long` as incompatible, though.
#if !defined(__GNUC__)
    template<> constexpr const char * TypeName<long> = "long";
#endif

    
    // Microsoft's C compiler defines std::size_t as an alias onto unsigned long long and throws an error here.
#if defined(__clang__) || (!defined(_MSC_VER) && !defined(__GNUC__))
    template<> constexpr const char * TypeName<Size_T> = "Size_T";
#endif

    
    template<> constexpr const char * TypeName<Real32>  = "R32";
    
    template<> constexpr const char * TypeName<Real64>  = "R64";
    
//    template<> constexpr const char * TypeName<Real128> = "R128";

    
    template<> constexpr const char * TypeName<Complex32>  = "C32";
    
    template<> constexpr const char * TypeName<Complex64>  = "C64";
    
//    template<> constexpr const char * TypeName<Complex128> = "C128";
    
} // namespace Tools
