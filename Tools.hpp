#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //define something for Windows (32-bit and 64-bit, this part is common)
    #include <io.h>
    #include <stdlib.h>
#else
    #include <unistd.h>
#endif

// Compatibility with non-clang and non-gcc compilers.
#ifndef __has_attribute
  #define __has_attribute(x) 0
#endif

#include <cstddef>
#include <deque>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>
#include <type_traits>
#include <memory>
#include <numeric>
#include <filesystem>
#include <utility>
#include <iomanip>
#include <mutex>
#include <functional>
#include <bit>


// In case somebody got the idea to ruin all other people's code.
#ifdef True
    #undef True
#endif

#ifdef False
    #undef False
#endif


// constexpr version of conditional ?-operator
#define COND( condition, case1, case2 ) ([&]{           \
        if constexpr ( condition )                      \
        {                                               \
            return case1;                               \
        }                                               \
        else                                            \
        {                                               \
            return case2;                               \
        }                                               \
    }())

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define TO_STD_STRING(x) std::string(STRINGIFY(x))
    
#define CONCAT_IMPL(id1, id2) id1##id2
#define CONCAT(id1, id2) CONCAT_IMPL(id1, id2)
    
#define CONCAT3_IMPL(id1, id2, id3) id1##id2##id3
#define CONCAT3(id1, id2, id3) CONCAT3_IMPL(id1, id2, id3)

#ifdef TOOLS_AGGRESSIVE_INLINING

    #if __has_attribute(__always_inline__)
        #define force_inline inline __attribute__((__always_inline__))

        #define force_flattening __attribute__((__flatten__))
    #else
        #define force_inline inline

        #define force_flattening
    #endif

#else

    #define force_inline inline

    #define force_flattening

#endif

// Define loop unrolling depending on the compiler
#ifdef TOOLS_AGGRESSIVE_UNROLLING

    #if defined(__ICC) || defined(__ICL)

        #define LOOP_UNROLL_FULL    _Pragma(STRINGIFY(unroll))
        #define LOOP_UNROLL(n)      _Pragma(STRINGIFY(unroll (n)))

    #elif defined(__clang__)

        #define LOOP_UNROLL_FULL    _Pragma(STRINGIFY(clang loop unroll(enable)))
        #define LOOP_UNROLL(n)      _Pragma(STRINGIFY(clang loop unroll_count(n)))

    #elif defined(__GNUC__) && !defined(__clang__)

        #define LOOP_UNROLL_FULL
        #define LOOP_UNROLL(n)      _Pragma(STRINGIFY(GCC unroll (n)))

    #elif defined(_MSC_BUILD)
        //  #pragma message ("Microsoft Visual C++ (MSVC) detected: Loop unrolling not supported!")
        #define LOOP_UNROLL_FULL
        #define LOOP_UNROLL(n)

    #else
        //  #warning "Unknown compiler: Loop unrolling not supported!"

        #define LOOP_UNROLL_FULL
        #define LOOP_UNROLL(n)
    #endif

#else
    
    #define LOOP_UNROLL_FULL
    #define LOOP_UNROLL(n)

#endif



//#define IS_ARITHMETIC(T) class = typename std::enable_if_t<std::is_arithmetic_v<T>>
//#define IS_FLOAT(T)      class = typename std::enable_if_t<std::is_floating_point_v<T>>
//#define IS_INT(T)        class = typename std::enable_if_t<std::is_integral_v<T>>
//
//#define IS_POSITIVE(x)  class = typename std::enable_if_t<x>0>
//
//#define ASSERT_ARITHMETIC(T) static_assert( std::is_arithmetic_v<T>, "Template parameter " #T " must be arithmetic type." );
//
//
//#define ASSERT_FLOAT(type) static_assert( std::is_floating_point_v<type>, "Template parameter " #type " must be floating point type." );


#if !defined(restrict)
    #if defined(__GNUC__)
        #define restrict __restrict__
        #define COMPILER_IS_ANAL_ABOUT_RESTRICT 1
    #elif defined(__clang__)
        #define restrict __restrict
        #define COMPILER_IS_ANAL_ABOUT_RESTRICT 0
    #elif defined(_MSC_VER)
        #define restrict __restrict
        #define COMPILER_IS_ANAL_ABOUT_RESTRICT 0
    #else
        #define COMPILER_IS_ANAL_ABOUT_RESTRICT 0
    #endif
#endif

namespace Tools
{
    // immutable, unaliased pointer to immutable type
    template<typename T> using cptr = const T * restrict const;

    // immutable, unaliased pointer to mutable type
    template<typename T> using mptr =       T * restrict const;

    // unaliased reference to immutable type
    template<typename T> using cref = const T & restrict;

    // unaliased reference to immutable type
    template<typename T> using mref =       T & restrict;
    
    using Size_T = std::size_t;
    
    static constexpr Size_T VarSize = 0;
    
    template<typename T>
    static constexpr bool IntQ = std::is_integral_v<T>;
    
#define ASSERT_INT(I) static_assert( IntQ<I>, "Template parameter " #I " must be an integral type." );
    
    template<typename T> 
    static constexpr bool UnsignedIntQ = std::is_unsigned_v<T> && std::is_integral_v<T>;
    
#define ASSERT_UINT(I) static_assert( UnsignedIntQ<I>, "Template parameter " #I " must be a unsigned integral type." );
    
    template<typename T>
    static constexpr bool SignedIntQ = std::is_signed_v<T> && std::is_integral_v<T>;
    
#define ASSERT_SIGNED_INT(I) static_assert( SignedIntQ<I>, "Template parameter " #I " must be a signed integral type." );
    
    template<typename T>
    static constexpr bool FloatQ = std::is_floating_point_v<T>;
    
    template<typename T>
    static constexpr bool ArithmeticQ = std::is_arithmetic_v<T>;
    
    template<> static constexpr bool ArithmeticQ<std::complex<float>>  = true;
    template<> static constexpr bool ArithmeticQ<std::complex<double>> = true;

    template <typename E>
    force_inline auto constexpr ToUnderlying( const E & e) noexcept
    {
        if constexpr( std::is_enum_v<E> )
        {
            return static_cast<typename std::underlying_type<E>::type>(e);
        }
        else
        {
            return e;
        }
    }

    
//    // Workaround for static_assert(false,"") within constexpr if.
    template <typename>
    inline constexpr bool DependentFalse = false;
    

#if ( (!(defined TOOLS_DEACTIVATE_VECTOR_EXTENSIONS)) && (__has_attribute(__ext_vector_type__)) )
    
    static constexpr bool vec_enabledQ = true;
    
    template<Size_T N, typename T>
    using vec_T = T __attribute__((__ext_vector_type__(N))) ;

    template<Size_T N, typename T>
    force_inline const T * get_ptr( const vec_T<N,T> & vec )
    {
        return reinterpret_cast<const T *>(&vec);
    }
    
    template<Size_T N, typename T>
    force_inline T * get_ptr( vec_T<N,T> & vec )
    {
        return reinterpret_cast<T *>(&vec);
    }
    
#else
    static constexpr bool vec_enabledQ = false;

    template<Size_T N, typename T>
    using vec_T = std::array<T,N>; //Just a dummy; will not be used, actually.
#endif

#if ( (!(defined TOOLS_DEACTIVATE_MATRIX_EXTENSIONS)) && (__has_attribute(__matrix_type__)) )
    
    static constexpr bool mat_enabledQ = true;

    template<Size_T M, Size_T N, typename T>
    using mat_T = T __attribute__((__matrix_type__(M,N))) ;
    
    template<Size_T M, Size_T N, typename T>
    force_inline const T * get_ptr( const mat_T<M,N,T> & mat )
    {
        return reinterpret_cast<const T *>(&mat);
    }
    
    template<Size_T M, Size_T N, typename T>
    force_inline T * get_ptr( mat_T<M,N,T> & mat )
    {
        return reinterpret_cast<T *>(&mat);
    }
    
#else
    static constexpr bool mat_enabledQ = false;

    template<Size_T M, Size_T N, typename T>
    using mat_T = std::array<std::array<T,N>,M>; //Just a dummy; will not be used, actually.
#endif
    
    
    // Dealing with the pain of signed conversions.
    
    template<typename Int>
    inline constexpr Size_T ToSize_T( const Int i )
    {
        static_assert(IntQ<Int>, "");
        
        return static_cast<Size_T>(std::max(Int(0),i));
    }
    
    inline constexpr Size_T ToSize_T( const Size_T i )
    {
        return i;
    }


} // namespace Tools

#include <complex>
#include "src/ct_string.hpp"
#include "src/ToString.hpp"
#include "src/combine_strings.hpp"
#include "src/Print.hpp"
#include "src/Profiler.hpp"
#include "src/Memory.hpp"



#include "src/Integers.hpp"
#include "src/BLAS_Enums.hpp"
#include "src/Scalars.hpp"
#include "src/BitFiddling.hpp"




#define _USE_MATH_DEFINES
#include <cmath>
#include "src/Math/Math.hpp"
#include "src/Math/ProbabilityDistributions.hpp"
#include "src/Math/BisectionSearch.hpp"
#include "src/Math/RegulaFalsi.hpp"
#include "src/Math/Kahan.hpp"
//#include "src/Math/CompensatedSummation.hpp"



//using Tensors::Op;
//using Tensors::AddTo;



#include "src/Scalars/combine_scalars.hpp"

// Common dependencies for combine_scalars_vec and combine_buffers_vec.
#include "src/Scalars/Op_vec.hpp"

// Overloads of Scalar::Op for complex types; uses vec_T.
//#include "src/Scalars/Op_complex.hpp.hpp"

// Overloads of combine_scalars for complex types; uses vec_T.
#include "src/Scalars/combine_scalars_vec.hpp"

// Overloads of combine_scalars for complex types; uses mat_T.
//#include "src/Scalars/combine_scalars_mat.hpp"

#include "src/TypeName.hpp"

#include "src/ParallelDo_Thread.hpp"

#include "src/ParallelAlgorithms.hpp"
#include "src/ParallelDo_Common.hpp"
#include "src/Do.hpp"

#include "src/BLAS_1.hpp"
#include "src/BLAS_2.hpp"

#include "src/AlignedAllocator.hpp"
#include "src/HeapArray.hpp"
#include "src/JobPointers.hpp"
#include "src/ParallelDo_JobPointers.hpp"

// TODO: get rid of std::vector here
#include "src/SortedList.hpp"


#include "src/CachedObject.hpp"

#include "src/Sort/SortNet.hpp"
//#include "src/Sort/BitonicSort.hpp"
//#include "src/Sort/BitonicMergeSort.hpp"
#include "src/Sort/Sort.hpp"

#include "src/Math/RealCubicSolve.hpp"
#include "src/Math/Det_Bareiss.hpp"
#include "src/Math/ProductAccumulator.hpp"

#include "src/Hashing.hpp"

#include "src/Debugging.hpp"



