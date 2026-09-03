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
            const auto loc = std::source_location::current();
            if( x <= static_cast<T>(0) )
            {
                eprint(std::string("assert_positive failed in function in ") + loc.file_name() + " at line "+ ToString(loc.line())+".");
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
        
        print("\nPrintCompilerIdentification()\n");
#ifdef __GNUC__
        valprint<true,a>("__GNUC__",true);
#else
        valprint<true,a>("__GNUC__",false);
#endif
        
#ifdef __clang__
        valprint<true,a>("__clang__",true);
#else
        valprint<true,a>("__clang__",false);
#endif
        
#ifdef __llvm__
        valprint<true,a>("__llvm__",true);
#else
        valprint<true,a>("__llvm__",false);
#endif
        
#ifdef __apple_build_version__
        valprint<true,a>("__apple_build_version__",true);
#else
        valprint<true,a>("__apple_build_version__",false);
#endif
        
#ifdef _MSC_VER
        valprint<true,a>("_MSC_VER",true);
#else
        valprint<true,a>("_MSC_VER",false);
#endif

#ifdef __INTEL_COMPILER
        valprint<true,a>("__INTEL_COMPILER",true);
#else
        valprint<true,a>("__INTEL_COMPILER",false);
#endif
        
#ifdef __ICC
        valprint<true,a>("__ICC",true);
#else
        valprint<true,a>("__ICC",false);
#endif
        
#ifdef __ICL
        valprint<true,a>("__ICL",true);
#else
        valprint<true,a>("__ICL",false);
#endif
        print("");
        
        constexpr Size_T b = 30;
        
#ifdef TOOLS_COMPILER_IS_GCC
        valprint<true,b>("TOOLS_COMPILER_IS_GCC",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_GCC",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_MSVC
        valprint<true,b>("TOOLS_COMPILER_IS_MSVC",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_MSVC",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_ICC
        valprint<true,b>("TOOLS_COMPILER_IS_ICC",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_ICC",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_CLANG
        valprint<true,b>("TOOLS_COMPILER_IS_CLANG",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_CLANG",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_APPLE_CLANG
        valprint<true,b>("TOOLS_COMPILER_IS_APPLE_CLANG",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_APPLE_CLANG",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_CLANGCL
        valprint<true,b>("TOOLS_COMPILER_IS_CLANGCL",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_CLANGCL",false);
#endif
        
#ifdef TOOLS_COMPILER_IS_ANY_CLANG
        valprint<true,b>("TOOLS_COMPILER_IS_ANY_CLANG",true);
#else
        valprint<true,b>("TOOLS_COMPILER_IS_ANY_CLANG",false);
#endif
        
        print("");
    }

    void PrintSettings()
    {
        constexpr Size_T a = 30;
        
        print("\nPrintSettings()\n");
        
#ifdef TOOLS_ENABLE_PROFILER
        valprint<true,a>("TOOLS_ENABLE_PROFILER",true);
#else
        valprint<true,a>("TOOLS_ENABLE_PROFILER",false);
#endif

#ifdef TOOLS_DEBUG
        valprint<true,a>("TOOLS_DEBUG",true);
#else
        valprint<true,a>("TOOLS_DEBUG",false);
#endif
        
#ifdef TOOLS_AGGRESSIVE_INLINING
        valprint<true,a>("TOOLS_AGGRESSIVE_INLINING",true);
#else
        valprint<true,a>("TOOLS_AGGRESSIVE_INLINING",false);
#endif
        
#ifdef TOOLS_AGGRESSIVE_UNROLLING
        valprint<true,a>("TOOLS_AGGRESSIVE_UNROLLING",true);
#else
        valprint<true,a>("TOOLS_AGGRESSIVE_UNROLLING",false);
#endif
        
#ifdef TOOLS_NO_RESTRICT
        valprint<true,a>("TOOLS_NO_RESTRICT",true);
#else
        valprint<true,a>("TOOLS_NO_RESTRICT",false);
#endif
        
#ifdef TOOLS_NO_PREFETCH
        valprint<true,a>("TOOLS_NO_PREFETCH",true);
#else
        valprint<true,a>("TOOLS_NO_PREFETCH",false);
#endif

#ifdef TOOLS_NO_VECTOR_EXTENSIONS
        valprint<true,a>("TOOLS_NO_VECTOR_EXTENSIONS",true);
#else
        valprint<true,a>("TOOLS_NO_VECTOR_EXTENSIONS",false);
#endif
        
#ifdef TOOLS_NO_MATRIX_EXTENSIONS
        valprint<true,a>("TOOLS_NO_MATRIX_EXTENSIONS",true);
#else
        valprint<true,a>("TOOLS_NO_MATRIX_EXTENSIONS",false);
#endif
        
        print("");
        
    }
    
} // namespace Tools
