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
        
        
    #define STRINGIFY(a) #a
    #define STRINGIFY2(a) STRINGIFY(a)
        
    #define TO_STD_STRING(x) std::string(STRINGIFY(x))
        
    #define CONCATT(id1, id2) id1##id2
    #define CONCAT(id1, id2) CONCATT(id1, id2)
        
    #define CONCATT3(id1, id2, id3) id1##id2##id3
    #define CONCAT3(id1, id2, id3) CONCATT3(id1, id2, id3)
        
        
    #define OMP_PLACES cores
    #define OMP_WAIT_POLICY = active
    #define OMP_DYNAMIC = false
        //#define OMP_PROC_BIND = true
    #define OMP_PROC_BIND spread // workers are spread across the available places to maximize the space inbetween two neighbouring threads
                                 //#define OMP_PROC_BIND close // worker threads are close to the master in contiguous partitions, e. g. if the master is occupying hardware thread 0, worker 1 will be placed on hw thread 1, worker 2 on hw thread 2 and so on
        
    #include <omp.h>
        
    #include "src/TypeName.hpp"
    #include "src/Timers.hpp"
    #include "src/Profiler.hpp"
    #include "src/Memory.hpp"
    #include "src/SortedList.hpp"
    #include "src/ParallelAlgorithms.hpp"
    #include "src/JobPointers.hpp"
    #include "src/BitFiddling.hpp"
        
    #define _USE_MATH_DEFINES
    #include <cmath>
    #include "src/MyMath/MyMath.hpp"
        
        
        //https://stackoverflow.com/a/43587319/8248900
        
    #define __ADD_CLONE_CODE_FOR_BASE_CLASS__(BASE)                                 \
    public:                                                                         \
    std::unique_ptr<BASE> Clone () const                                        \
    {                                                                           \
    return std::unique_ptr<BASE>(CloneImplementation());                    \
    }                                                                           \
    private:                                                                        \
    virtual BASE * CloneImplementation() const = 0;
        
    #define __ADD_CLONE_CODE_FOR_ABSTRACT_CLASS__(CLASS)                            \
    public:                                                                         \
    std::unique_ptr<CLASS> Clone () const                                       \
    {                                                                           \
    return std::unique_ptr<CLASS>(CloneImplementation());                   \
    }                                                                           \
    private:                                                                        \
    virtual CLASS * CloneImplementation() const override = 0;
        
        
    #define __ADD_CLONE_CODE__(DERIVED)                                             \
    public:                                                                         \
    std::unique_ptr<DERIVED> Clone () const                                     \
    {                                                                           \
    return std::unique_ptr<DERIVED>(CloneImplementation());                 \
    }                                                                           \
    \
    private:                                                                        \
    virtual DERIVED * CloneImplementation() const override                      \
    {                                                                           \
    return new DERIVED(*this);                                              \
    }
        
        
        
        
    #define IsArithmetic(T) class = typename std::enable_if_t<std::is_arithmetic_v<T>>
    #define IsFloat(T)      class = typename std::enable_if_t<std::is_floating_point_v<T>>
        //#define IsInt(I)        class = typename std::enable_if_t<std::is_signed_v<I> && std::is_integral_v<I>>
    #define IsInt(I)        class = typename std::enable_if_t<std::is_integral_v<I>>
    #define IsPositive(x)   class = typename std::enable_if_t<x>0>
        
    #define ASSERT_ARITHMETIC(T) static_assert( std::is_arithmetic_v<T>, "Template parameter " #T " must be arithmetic type." );
        
    #define ASSERT_INT(I) static_assert( std::is_signed_v<I> && std::is_integral_v<I>, "Template parameter " #I " must be integral type." );
        
    #define ASSERT_FLOAT(type) static_assert( std::is_floating_point_v<type>, "Template parameter " #type " must be floating point type." );

#endif

// constexpr version of conditional ?-operator
#define COND( condition, case1, case2 ) [&]{                \
            if constexpr ( condition )                      \
            {                                               \
                return case1;                               \
            }                                               \
            else                                            \
            {                                               \
                return case2;                               \
            }                                               \
        }()


namespace Tools
{
    template <class Enum>
    auto to_underlying(Enum e)
    {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }
}
