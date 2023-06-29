#pragma once

namespace Tools
{
    
    // length of cache line measured in bytes
    static constexpr Size_T CACHE_LINE_WIDTH = 64;
    //TODO: Replace the hard-coded constant by __cpp_lib_hardware_interference_size as soon as this C++17 feature is supported by all compilers.

    static constexpr Size_T MEMORY_PADDING = 1;

    #if !defined(restrict)
        #if defined(__GNUC__)
            #define restrict __restrict__
            #define COMPILER_IS_NAZI_ABOUT_RESTRICT 1
        #elif defined(__clang__)
            #define restrict __restrict__
            #define COMPILER_IS_NAZI_ABOUT_RESTRICT 0
        #elif defined(_MSC_VER)
            #define restrict __restrict
            #define COMPILER_IS_NAZI_ABOUT_RESTRICT 0
        #else
            #define COMPILER_IS_NAZI_ABOUT_RESTRICT 0
        #endif
    #endif

    #if !defined(prefetch)
        #if defined(__GNUC__) || defined(__clang__)
            #define prefetch __builtin_prefetch
        #endif
    #endif

    //#define SAFE_ALLOCATE_WARNINGS

    static constexpr Size_T ALIGNMENT = 8;

    #if defined(__GNUC__) || defined(__clang__)
        #define ALIGNED __attribute__((aligned(ALIGNMENT)))
    #else
        #define ALIGNED
    #endif

    static constexpr Size_T OBJECT_ALIGNMENT = 2 * CACHE_LINE_WIDTH;

    #ifndef PREFETCH_STRIDE
        #define PREFETCH_STRIDE (4*CACHE_LINE_WIDTH)
    #endif

    //    // immutable, unaliased pointer to immutable type
    //    template<typename T> using ptr = const T * restrict const;
    //
    //    // immutable, unaliased pointer to mutable type
    //    template<typename T> using mut =       T * restrict const;
        
    // immutable, unaliased pointer to immutable type
    template<typename T> using ptr = const T * restrict const;
    
    // immutable, unaliased pointer to mutable type
    template<typename T> using mut =       T * restrict const;
    

    
    template<typename Int>
    force_inline constexpr Int CeilDivide( const Int n, const Int b )
    {
        return ( (n + b - Int(1) ) / b );
    }


    template<typename Int>
    force_inline constexpr Int RoundUpTo( const Int n, const Int b )
    {
        return CeilDivide(n,b) * b;
    }
    
    
    template<typename Int>
    force_inline constexpr Int FloorDivide( const Int n, const Int b )
    {
        return n / b;
    }
    
    template<typename Int>
    force_inline constexpr Int RoundDownTo( const Int n, const Int b )
    {
        return FloorDivide(n,b) * b;
    }
    
    // Computes k-th job pointer for job_count equally sized jobs distributed on thread_count threads.
    template<typename Int, typename Int1, typename Int2>
    force_inline Int JobPointer( const Int job_count, const Int1 thread_count, const Int2 k )
    {
        return job_count/static_cast<Int>(thread_count)*static_cast<Int>(k) + job_count%static_cast<Int>(thread_count)*static_cast<Int>(k)/static_cast<Int>(thread_count);
    }
    
    inline bool is_aligned( const void * const pointer, const Size_T byte_count = ALIGNMENT )
    {
        return reinterpret_cast<std::uintptr_t>(pointer) % byte_count == 0;
    }
    
    force_inline void * aligned_malloc( const Size_T size, const Size_T alignment )
    {
        const Size_T padded_size = ( (size - 1) / alignment + 1 ) * alignment;
//        const Size_T padded_size = RoundUpTo(size, alignment);
        
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        void * ptr_ = _aligned_malloc(padded_size, alignment);
#else
        void * ptr_ = std::aligned_alloc(alignment, padded_size);
#endif
        
        if( ptr_ == nullptr )
        {
            eprint("aligned_malloc: failed to allocate memory.");
            dump(padded_size);
            return nullptr;
        }
        else
        {
            return ptr_;
        }
    }

    force_inline void aligned_free( void * ptr_ )
    {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            _aligned_free( ptr_ );
        #else
            free( ptr_ );
        #endif
    }
    
    
    
    template <typename T>
    force_inline int safe_free( T * & ptr_ )
    {
        int wasallocated = (ptr_ != nullptr);
        if( wasallocated )
        {
            aligned_free(ptr_);
            ptr_ = nullptr;
            
        }
        return !wasallocated;
    }
    
#if COMPILER_IS_NAZI_ABOUT_RESTRICT
    // overload function for restrict qualifier
    template <typename T>
    force_inline int safe_free( T * restrict & ptr_ )
    {
        int wasallocated = (ptr_ != nullptr);
        if( wasallocated )
        {
            aligned_free(ptr_); ptr_ = nullptr;
        }
        return !wasallocated;
    }
#endif
    
    
    template <typename T>
    force_inline int safe_alloc( T * & ptr_, const Size_T n )
    {
        int wasallocated = (ptr_ != nullptr);
        
        if( wasallocated != 0 )
        {
#ifdef SAFE_ALLOCATE_WARNINGS
            wprint("safe_alloc: Pointer was not NULL. Calling safe_free to prevent memory leak.");
#endif
            safe_free(ptr_);
        }
        
        ptr_ = static_cast<T *>( aligned_malloc( n * sizeof(T), ALIGNMENT ) );

        return wasallocated;
    }
    
#if COMPILER_IS_NAZI_ABOUT_RESTRICT
    // overload function for restrict qualifier
    template <typename T>
    force_inline int safe_alloc( T * restrict & ptr_, const Size_T n )
    {
        int wasallocated = (ptr_ != nullptr);
        
        if( wasallocated != 0 )
        {
        #ifdef SAFE_ALLOCATE_WARNINGS
            wprint("safe_alloc: Pointer was not NULL. Calling safe_free to prevent memory leak.");
        #endif
            safe_free(ptr_);
        }
        
        ptr_ = static_cast<T *>( aligned_malloc( n * sizeof(T), ALIGNMENT ) );
        
        return wasallocated;
    }
#endif
    
    template<int readwrite, int locality, typename T>
    force_inline void prefetch_buffer( ptr<T> begin, const Size_T n )
    {
        const Size_T prefetch_size = n * sizeof(T);
        
        const unsigned char * ptr_ = reinterpret_cast<const unsigned char*>(begin);
    
        for( Size_T offset = 0; offset < prefetch_size; offset += PREFETCH_STRIDE )
        {
            prefetch( &ptr_[offset], readwrite, locality );
        }
    }
    
    template<Size_T N, int readwrite, int locality, typename T>
    force_inline void prefetch_buffer( ptr<T> begin )
    {
        constexpr Size_T PREFETCH_SIZE = N * sizeof(T);
        
        const unsigned char * ptr_ = reinterpret_cast<const unsigned char*>(begin);
    
        LOOP_UNROLL_FULL
        for( Size_T offset = 0; offset < PREFETCH_SIZE; offset += PREFETCH_STRIDE )
        {
            prefetch( &ptr_[offset], readwrite, locality );
        }
    }
    
} // namespace Tools
