#pragma once

namespace Tools
{
    // From https://stackoverflow.com/a/49658950/8248900.
    template<IntQ Int_1, IntQ Int_0>
    TOOLS_FORCE_INLINE constexpr Int_1 int_cast( const Int_0 n )
    {
        if( !std::in_range<Int_1>( n ) )
        {
            eprint(std::string("int_cast<") + TypeName<Int_1> + "," + TypeName<Int_0> + "> reports integer overflow for n = " + ToString(n) + ".");
            
            return 0;
        }
        
        return static_cast<Int_1>(n);
    }
    
    
    template<typename T>
    TOOLS_FORCE_INLINE void assert_positive( const T x )
    {
#ifdef TOOLS_DEBUG
        if constexpr ( std::numeric_limits<T>::is_signed )
        {
            if( x <= static_cast<T>(0) )
            {
                eprint(std::string("assert_positive failed in function in ") + std::string(__FILE__) + " at line "+ ToString(__LINE__)+".");
            }
        }
#else
        (void)x;
#endif
    }
    
    
    void Check_is_pointer()
    {
        
        print("\nCheck_is_pointer()\n");
#ifdef TOOLS_NO_RESTRICT
        print("TOOLS_NO_RESTRICT is defined.\n");
#else
        print("TOOLS_NO_RESTRICT is undefined.\n");
#endif
        
        TOOLS_DUMP( std::is_pointer<bool *>::value );
        TOOLS_DUMP( std::is_pointer<bool * const>::value );
        TOOLS_DUMP( std::is_pointer<const bool * const>::value );
        print("");
        TOOLS_DUMP( std::is_pointer<bool * TOOLS_RESTRICT>::value );
        TOOLS_DUMP( std::is_pointer<bool * const TOOLS_RESTRICT>::value );
        TOOLS_DUMP( std::is_pointer<const bool * const TOOLS_RESTRICT>::value );
        print("");
        TOOLS_DUMP( std::is_pointer<bool * TOOLS_RESTRICT>::value );
        TOOLS_DUMP( std::is_pointer<bool * TOOLS_RESTRICT const>::value );
        TOOLS_DUMP( std::is_pointer<const bool * TOOLS_RESTRICT const>::value );
    }
    
    void PrintCompilerIdentification()
    {
        constexpr Size_T a = 30;
        
        print("\PrintCompilerIdentification()\n");
#ifdef __GNUC__
        valprint<a>("__GNUC__",true);
#else
        valprint<a>("__GNUC__",false);
#endif
        
#ifdef __clang__
        valprint<a>("__clang__",true);
#else
        valprint<a>("__clang__",false);
#endif
        
#ifdef __llvm__
        valprint<a>("__llvm__",true);
#else
        valprint<a>("__llvm__",false);
#endif
        
#ifdef __apple_build_version__
        valprint<a>("__apple_build_version__",true);
#else
        valprint<a>("__apple_build_version__",false);
#endif
        
#ifdef _MSC_VER
        valprint<a>("_MSC_VER",true);
#else
        valprint<a>("_MSC_VER",false);
#endif

#ifdef __INTEL_COMPILER
        valprint<a>("__INTEL_COMPILER",true);
#else
        valprint<a>("__INTEL_COMPILER",false);
#endif
        
#ifdef __ICC
        valprint<a>("__ICC",true);
#else
        valprint<a>("__ICC",false);
#endif
        
#ifdef __ICL
        valprint<a>("__ICL",true);
#else
        valprint<a>("__ICL",false);
#endif
        print("");
        
        constexpr Size_T b = 30;
        
#ifdef TOOLS_COMPILER_IS_GCC
        valprint<b>("TOOLS_COMPILER_IS_GCC",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_GCC",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_MSVC
        valprint<b>("TOOLS_COMPILER_IS_MSVC",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_MSVC",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_ICC
        valprint<b>("TOOLS_COMPILER_IS_ICC",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_ICC",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_CLANG
        valprint<b>("TOOLS_COMPILER_IS_CLANG",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_CLANG",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_APPLE_CLANG
        valprint<b>("TOOLS_COMPILER_IS_APPLE_CLANG",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_APPLE_CLANG",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_CLANGCL
        valprint<b>("TOOLS_COMPILER_IS_CLANGCL",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_CLANGCL",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_ANY_CLANG
        valprint<b>("TOOLS_COMPILER_IS_ANY_CLANG",true);
#else
        valprint<b>("TOOLS_COMPILER_IS_ANY_CLANG",false);
#endif
        
        print("");
    }

    void PrintSettings()
    {
        constexpr Size_T a = 30;
        
        print("\nPrintSettings()\n");
        
#ifdef TOOLS_ENABLE_PROFILER
        valprint<a>("TOOLS_ENABLE_PROFILER",true);
#else
        valprint<a>("TOOLS_ENABLE_PROFILER",false);
#endif

#ifdef TOOLS_DEBUG
        valprint<a>("TOOLS_DEBUG",true);
#else
        valprint<a>("TOOLS_DEBUG",false);
#endif
        
#ifdef TOOLS_AGGRESSIVE_INLINING
        valprint<a>("TOOLS_AGGRESSIVE_INLINING",true);
#else
        valprint<a>("TOOLS_AGGRESSIVE_INLINING",false);
#endif
        
#ifdef TOOLS_AGGRESSIVE_UNROLLING
        valprint<a>("TOOLS_AGGRESSIVE_UNROLLING",true);
#else
        valprint<a>("TOOLS_AGGRESSIVE_UNROLLING",false);
#endif
        
#ifdef TOOLS_NO_RESTRICT
        valprint<a>("TOOLS_NO_RESTRICT",true);
#else
        valprint<a>("TOOLS_NO_RESTRICT",false);
#endif
        
#ifdef TOOLS_NO_PREFECT
        valprint<a>("TOOLS_NO_PREFECT",true);
#else
        valprint<a>("TOOLS_NO_PREFECT",false);
#endif

#ifdef TOOLS_NO_VECTOR_EXTENSIONS
        valprint<a>("TOOLS_NO_VECTOR_EXTENSIONS",true);
#else
        valprint<a>("TOOLS_NO_VECTOR_EXTENSIONS",false);
#endif
        
#ifdef TOOLS_NO_MATRIX_EXTENSIONS
        valprint<a>("TOOLS_NO_MATRIX_EXTENSIONS",true);
#else
        valprint<a>("TOOLS_NO_MATRIX_EXTENSIONS",false);
#endif
        
        print("");
        
    }
    
} // namespace Tools
