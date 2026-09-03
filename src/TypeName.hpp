#pragma once

#include <stdint.h>
#include <string>

namespace Tools
{
    // Taken from https://stackoverflow.com/a/4485051/8248900
    
    
    // default implementation
    template<typename T>
    constexpr auto TypeName = ct_string("UnknownType");
    
    template<> constexpr auto TypeName<Real32>      = ct_string("R32");
    template<> constexpr auto TypeName<Real64>      = ct_string("R64");
    template<> constexpr auto TypeName<Complex32>   = ct_string("C32");
    template<> constexpr auto TypeName<Complex64>   = ct_string("C64");
    template<> constexpr auto TypeName<std::string> = ct_string("String");
    
    template<typename S, typename T> auto TypeName<std::pair<S,T>> = ct_string("pair<") + TypeName<S> + "," + TypeName<T> + ">";
    
    
    template<typename T> requires std::is_integral_v<T>
    constexpr auto TypeName<T> = Scalar::IntegralTypeTraits<T>::type_name;
    
    // default implementation
    template<typename T>
    constexpr auto FullTypeName = ct_string("UnknownType");
    
    template<> constexpr auto FullTypeName<Real32>    = ct_string("float");
    template<> constexpr auto FullTypeName<Real64>    = ct_string("double");
    template<> constexpr auto FullTypeName<Complex32> = ct_string("std::complex<float>");
    
    template<> constexpr auto FullTypeName<Complex64> = ct_string("std::complex<double>");
    
    template<> constexpr auto FullTypeName<std::string> = ct_string("std::string");
    
    template<typename S, typename T>
    constexpr auto FullTypeName<std::pair<S,T>> = ct_string("pair<" + FullTypeName<S> + "," + FullTypeName<T> + ">");
    
    template<typename T> requires std::is_integral_v<T>
    constexpr auto FullTypeName<T> = Scalar::IntegralTypeTraits<T>::full_name;

    [[maybe_unused]] std::string IntegerInfo()
    {
        std::stringstream s ;

        const int  w0 = 22 + 1;
        const int  w1 = 11;
        const int  w2 = 22;
        const int  w3 = 5;
        const int  w4 = 5;
        
        
        auto line = [=,&s]<typename T>( const char * name )
        {
            s << std::right << std::setw(w0) << name << " | "
              << std::right << std::setw(w1) << TypeName<T> << " | "
              << std::right << std::setw(w2) << FullTypeName<T> << " | "
              << std::right << std::setw(w3) << 8 * sizeof(T) << " | "
              << std::right << std::setw(w4) << sizeof(T) << " \n";
        };
        
        s << "\n";
        s << std::right << std::setw(w0) << "Internal name" << " | "
          << std::right << std::setw(w1) << "TypeName" << " | "
          << std::right << std::setw(w2) << "FullTypeName" << " | "
          << std::right << std::setw(w3) << "Bits" << " | "
          << std::right << std::setw(w4) << "Bytes" << " \n";
       
        s << std::string( w0, '-' ) << "-+-"
          << std::string( w1, '-' ) << "-+-"
          << std::string( w2, '-' ) << "-+-"
          << std::string( w3, '-' ) << "-+-"
          << std::string( w4, '-' ) << "-\n";
        
        line.operator()<                  Int8>( "                  Int8" );
        line.operator()<                 Int16>( "                 Int16" );
        line.operator()<                 Int32>( "                 Int32" );
        line.operator()<                 Int64>( "                 Int64" );
        
        line.operator()<                 UInt8>( "                 UInt8" );
        line.operator()<                UInt16>( "                UInt16" );
        line.operator()<                UInt32>( "                UInt32" );
        line.operator()<                UInt64>( "                UInt64" );
        
        
        s << std::string( w0, ' ' ) << " | "
          << std::string( w1, ' ' ) << " | "
          << std::string( w2, ' ' ) << " | "
          << std::string( w3, ' ' ) << " | "
          << std::string( w4, ' ' ) << " \n";
        
        line.operator()<                  bool>( "                  bool" );
        line.operator()<                  char>( "                  char" );
        line.operator()<                size_t>( "                size_t" );
        
        s << std::string( w0, ' ' ) << " | "
          << std::string( w1, ' ' ) << " | "
          << std::string( w2, ' ' ) << " | "
          << std::string( w3, ' ' ) << " | "
          << std::string( w4, ' ' ) << " \n";
        
        line.operator()<  signed          char>( "  signed          char" );
        line.operator()<  signed short     int>( "  signed short     int" );
        line.operator()<  signed           int>( "  signed           int" );
        line.operator()<  signed long      int>( "  signed long      int" );
        line.operator()<  signed long long int>( "  signed long long int" );
        
        s << std::string( w0, ' ' ) << " | "
          << std::string( w1, ' ' ) << " | "
          << std::string( w2, ' ' ) << " | "
          << std::string( w3, ' ' ) << " | "
          << std::string( w4, ' ' ) << " \n";
        
        line.operator()<unsigned          char>( "unsigned          char" );
        line.operator()<unsigned short     int>( "unsigned short     int" );
        line.operator()<unsigned           int>( "unsigned           int" );
        line.operator()<unsigned long      int>( "unsigned long      int" );
        line.operator()<unsigned long long int>( "unsigned long long int" );
        
        s << std::string( w0, ' ' ) << " | "
          << std::string( w1, ' ' ) << " | "
          << std::string( w2, ' ' ) << " | "
          << std::string( w3, ' ' ) << " | "
          << std::string( w4, ' ' ) << " \n";
        
        return s.str();
    }
    
    [[maybe_unused]] std::string FloatingPointInfo()
    {
        std::stringstream s ;

        const int  w0 = 22 + 1;
        const int  w1 = 11;
        const int  w2 = 22;
        const int  w3 = 5;
        const int  w4 = 5;
        const int  w5 = 4;
        const int  w6 = 7;
        
        
        auto line = [=,&s]<typename T>( const char * name )
        {
            s << std::right << std::setw(w0) << name << " | "
              << std::right << std::setw(w1) << TypeName<T> << " | "
              << std::right << std::setw(w2) << FullTypeName<T> << " | "
              << std::right << std::setw(w3) << 8 * sizeof(T) << " | "
              << std::right << std::setw(w4) << sizeof(T) << " | "
              << std::right << std::setw(w5) << TypeName<Scalar::Real<T>> << " | "
              << std::right << std::setw(w6) << TypeName<Scalar::Complex<T>> << " \n";
        };
        
        s << "\n";
        s << std::right << std::setw(w0) << "Internal name" << " | "
          << std::right << std::setw(w1) << "TypeName" << " | "
          << std::right << std::setw(w2) << "FullTypeName" << " | "
          << std::right << std::setw(w3) << "Bits" << " | "
          << std::right << std::setw(w4) << "Bytes" << " | "
          << std::right << std::setw(w5) << "Real" << " | "
          << std::right << std::setw(w6) << "Complex" << " \n";
       
        s << std::string( w0, '-' ) << "-+-"
          << std::string( w1, '-' ) << "-+-"
          << std::string( w2, '-' ) << "-+-"
          << std::string( w3, '-' ) << "-+-"
          << std::string( w4, '-' ) << "-+-"
          << std::string( w5, '-' ) << "-+-"
          << std::string( w6, '-' ) << "-\n";
        
        line.operator()<                Real32>( "                Real32" );
        line.operator()<                Real64>( "                Real64" );
        line.operator()<             Complex32>( "             Complex32" );
        line.operator()<             Complex64>( "             Complex64" );
        
        s << std::string( w0, ' ' ) << " | "
          << std::string( w1, ' ' ) << " | "
          << std::string( w2, ' ' ) << " | "
          << std::string( w3, ' ' ) << " | "
          << std::string( w4, ' ' ) << " | "
          << std::string( w5, ' ' ) << " | "
          << std::string( w6, ' ' ) << " \n";
        
        return s.str();
    }
    
    
    
    
    
    // https://stackoverflow.com/a/56766138/8248900
    
    template <typename T>
    constexpr auto PrettyTypeName()
    {
        std::string_view name;
        std::string_view prefix;
        std::string_view suffix;
    #if defined(TOOLS_COMPILER_IS_ANY_CLANG)
        name = __PRETTY_FUNCTION__;
        prefix = "auto Tools::PrettyTypeName() [T = ";
        suffix = "]";
    #elif defined(TOOLS_COMPILER_IS_GCC)
        name = __PRETTY_FUNCTION__;
        prefix = "constexpr auto Tools::PrettyTypeName() [with T = ";
        suffix = "]";
    #elif defined(TOOLS_COMPILER_IS_MSVC)
        name = __FUNCSIG__;
        prefix = "auto __cdecl Tools::PrettyTypeName<";
        suffix = ">(void)";
    #elif
        name = "UnknownFunction";
        prefix = "";
        suffix = "";
    #endif
        name.remove_prefix(prefix.size());
        name.remove_suffix(suffix.size());
        return name;
    }
    
    
} // namespace Tools
