#ifndef SCHUMACHER_TOOLS_HPP
    #define SCHUMACHER_TOOLS_HPP

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        //define something for Windows (32-bit and 64-bit, this part is common)
        #include <io.h>
        #include <stdlib.h>
    #else
        #include <unistd.h>
    #endif
    

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

        
    #define STRINGIFY(a) #a
    #define STRINGIFY2(a) STRINGIFY(a)
        
    #define TO_STD_STRING(x) std::string(STRINGIFY(x))
        
    #define CONCATT(id1, id2) id1##id2
    #define CONCAT(id1, id2) CONCATT(id1, id2)
        
    #define CONCATT3(id1, id2, id3) id1##id2##id3
    #define CONCAT3(id1, id2, id3) CONCATT3(id1, id2, id3)

#if defined(__GNUC__) || defined(__clang__) // force_inline
    #define force_inline inline __attribute__((always_inline))

    #define force_flattening __attribute__((flatten))
#else
    #define force_inline inline

    #define force_flattening
#endif



// Define loop unrolling depending on the compiler
#if defined(__ICC) || defined(__ICL)
  #define LOOP_UNROLL(n)      _Pragma(STRINGIFY2(unroll (n)))
#elif defined(__clang__)
  #define LOOP_UNROLL(n)      _Pragma(STRINGIFY2(clang loop unroll_count(n)))
#elif defined(__GNUC__) && !defined(__clang__)
  #define LOOP_UNROLL(n)      _Pragma(STRINGIFY2(GCC unroll (n)))
#elif defined(_MSC_BUILD)
//  #pragma message ("Microsoft Visual C++ (MSVC) detected: Loop unrolling not supported!")
  #define LOOP_UNROLL(n)
#else
//  #warning "Unknown compiler: Loop unrolling not supported!"
  #define LOOP_UNROLL(n)
#endif

// Define loop unrolling depending on the compiler
#if defined(__ICC) || defined(__ICL)
  #define LOOP_UNROLL_FULL      _Pragma(STRINGIFY2(unroll))
#elif defined(__clang__)
  #define LOOP_UNROLL_FULL      _Pragma(STRINGIFY2(clang loop unroll(enable)))
#elif defined(__GNUC__) && !defined(__clang__)
//  #define LOOP_UNROLL_FULL      _Pragma(STRINGIFY2(GCC unroll (16)))
  #define LOOP_UNROLL_FULL
#elif defined(_MSC_BUILD)
//  #pragma message ("Microsoft Visual C++ (MSVC) detected: Loop unrolling not supported!")
  #define LOOP_UNROLL_FULL
#else
//  #warning "Unknown compiler: Loop unrolling not supported!"
  #define LOOP_UNROLL_FULL
#endif

#define IS_ARITHMETIC(T) class = typename std::enable_if_t<std::is_arithmetic_v<T>>
#define IS_FLOAT(T)      class = typename std::enable_if_t<std::is_floating_point_v<T>>
#define IS_INT(T)        class = typename std::enable_if_t<std::is_integral_v<T>>

#define IS_POSITIVE(x)  class = typename std::enable_if_t<x>0>

#define ASSERT_ARITHMETIC(T) static_assert( std::is_arithmetic_v<T>, "Template parameter " #T " must be arithmetic type." );

#define ASSERT_INT(I) static_assert( std::is_integral_v<I>, "Template parameter " #I " must be an integral type." );

#define ASSERT_SIGNED_INT(I) static_assert( std::is_signed_v<I> && std::is_integral_v<I>, "Template parameter " #I " must be a signed integral type." );

#define ASSERT_UINT(I) static_assert( std::is_unsigned_v<I> && std::is_integral_v<I>, "Template parameter " #I " must be a unsigned integral type." );
#define ASSERT_UINT(I) static_assert( std::is_unsigned_v<I> && std::is_integral_v<I>, "Template parameter " #I " must be a unsigned integral type." );

#define ASSERT_FLOAT(type) static_assert( std::is_floating_point_v<type>, "Template parameter " #type " must be floating point type." );


namespace Tools
{
    using Size_T = std::size_t;
    
    
    
#if defined(__clang__)
    #if ( __has_attribute(ext_vector_type) )
        static constexpr bool vec_enabledQ = true;
    
        template<Size_T N, typename T>
        using vec_T = T __attribute__((__ext_vector_type__(N))) ;
    #endif
#else
    static constexpr bool vec_enabledQ = false;
    
    template<Size_T N, typename T>
    using vec_T = std::array<T,N>; //Just a dummy; will not be used, actually.
#endif
    
#if defined(__clang__)
    #if ( __has_attribute(matrix_type) )
        static constexpr bool mat_enabledQ = true;
    
        template<Size_T M, Size_T N, typename T>
        using mat_T = T __attribute__((matrix_type(M,N))) ;
    #endif
#else
    static constexpr bool mat_enabledQ = false;
    
    template<Size_T M, Size_T N, typename T>
    using mat_T = std::array<std::array<T,N>,M>; //Just a dummy; will not be used, actually.
#endif

    
}

    #include <complex>
    #include "src/ToString.hpp"
    #include "src/Print.hpp"
    #include "src/Profiler.hpp"
    #include "src/Scalars.hpp"
    #include "src/BitFiddling.hpp"
    #include "src/Memory.hpp"

    

    #define _USE_MATH_DEFINES
    #include <cmath>
    #include "src/MyMath/MyMath.hpp"
    #include "src/MyMath/ProbabilityDistributions.hpp"
    #include "src/MyMath/BisectionSearch.hpp"
    #include "src/MyMath/RegulaFalsi.hpp"

    #include "src/BLAS_Enums.hpp"

    using Tensors::Op;
    using Tensors::AddTo;

    #include "src/Scalars/combine_scalars.hpp"
    #include "src/TypeName.hpp"
    

    #include "src/ParallelDo_Thread.hpp"

    #include "src/ParallelAlgorithms.hpp"
    #include "src/JobPointers.hpp"
    #include "src/ParallelDo_Common.hpp"
    #include "src/Do.hpp"

    #include "src/BLAS_1.hpp"

    #include "src/SortedList.hpp"


    #include "src/CachedObject.hpp"

    #include "src/Sort.hpp"

    #include "src/MyMath/RealCubicSolve.hpp"

        //https://stackoverflow.com/a/43587319/8248900
        
    #define __ADD_CLONE_CODE_FOR_BASE_CLASS__(BASE)                                 \
    public:                                                                         \
        [[nodiscard]] std::shared_ptr<BASE> Clone () const                          \
        {                                                                           \
            return std::shared_ptr<BASE>(CloneImplementation());                    \
        }                                                                           \
    private:                                                                        \
        [[nodiscard]] virtual BASE * CloneImplementation() const = 0;
        
    #define __ADD_CLONE_CODE_FOR_ABSTRACT_CLASS__(CLASS)                            \
    public:                                                                         \
        [[nodiscard]] std::shared_ptr<CLASS> Clone () const                         \
        {                                                                           \
            return std::shared_ptr<CLASS>(CloneImplementation());                   \
        }                                                                           \
    private:                                                                        \
        [[nodiscard]] virtual CLASS * CloneImplementation() const override = 0;
        
        
    #define __ADD_CLONE_CODE__(DERIVED)                                             \
    public:                                                                         \
        [[nodiscard]] std::shared_ptr<DERIVED> Clone () const                       \
        {                                                                           \
            return std::shared_ptr<DERIVED>(CloneImplementation());                 \
        }                                                                           \
                                                                                    \
    private:                                                                        \
        [[nodiscard]] virtual DERIVED * CloneImplementation() const override        \
        {                                                                           \
            return new DERIVED(*this);                                              \
        }


    namespace Tools
    {
        
        template <typename E>
        constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept
        {
            return static_cast<typename std::underlying_type<E>::type>(e);
        }

    }

#endif


