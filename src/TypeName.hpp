#pragma once

#include <stdint.h>

namespace Tools
{
    // Taken from https://stackoverflow.com/a/4485051/8248900
    
    // default implementation
    template<typename T>
    constexpr const char * TypeName = "UnknownType";
    
    template<> constexpr const char * TypeName<bool> = "Bool";
    
    template<> constexpr const char * TypeName<Int8> = "I8";
    
    template<> constexpr const char * TypeName<Int16> = "I16";
    
    template<> constexpr const char * TypeName<Int32> = "I32";
    
    template<> constexpr const char * TypeName<Int64> = "I64";
    
    template<> constexpr const char * TypeName<UInt8> = "U8";
    
    template<> constexpr const char * TypeName<UInt16> = "U16";
    
    template<> constexpr const char * TypeName<UInt32> = "U32";
    
    template<> constexpr const char * TypeName<UInt64> = "U64";
    

    // For some compilers long int is just a typedef of another type.
    // For some compilers it's not.
    template<> constexpr std::enable_if<
        !std::is_same<long int,Int64>::value
        &&
        !std::is_same<long int,Int32>::value
        ,
        const char *
    >::type TypeName<long int> = "L";
    
    // For some compilers std::size_t is just a typedef of another type.
    // For some compilers it's not.
    template<> constexpr std::enable_if<
        !std::is_same<Size_T,UInt64>::value
        &&
        !std::is_same<Size_T,UInt32>::value
        &&
        !std::is_same<Size_T,UInt16>::value
        &&
        !std::is_same<Size_T,UInt16>::value
        ,
        const char *
    >::type TypeName<Size_T> = "Size_T";

    
    template<> constexpr const char * TypeName<Real32>  = "R32";
    
    template<> constexpr const char * TypeName<Real64>  = "R64";
    
//    template<> constexpr const char * TypeName<Real128> = "R128";

    
    template<> constexpr const char * TypeName<Complex32>  = "C32";
    
    template<> constexpr const char * TypeName<Complex64>  = "C64";
    
    template<typename S, typename T> const char * TypeName<std::pair<S,T>> = "pair<?,?>";
    
//    template<> constexpr const char * TypeName<Complex128> = "C128";
    
} // namespace Tools
