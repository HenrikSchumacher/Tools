#pragma once

// length of cache line measured in bytes
#define CACHE_LINE_WIDTH 64
//TODO: Replace the hard-coded constant by __cpp_lib_hardware_interference_size as soon as this C++17 feature is supported by all compilers.

#define MEMORY_PADDING 1

#if !defined(restrict)
    #define restrict __restrict
#endif

#if !defined(prefetch)
    #define prefetch __builtin_prefetch
#endif

//#define SAFE_ALLOCATE_WARNINGS

#define ALIGNMENT 8

#define ALIGNED __attribute__((aligned(8)))

#define OBJECT_ALIGNMENT 2 * CACHE_LINE_WIDTH

#ifndef PREFETCH_STRIDE
    #define PREFETCH_STRIDE (4*CACHE_LINE_WIDTH)
#endif

namespace Tools
{
    
    inline bool is_aligned( const void * restrict const pointer, const size_t byte_count = ALIGNMENT )
    {
        return reinterpret_cast<std::uintptr_t>(pointer) % byte_count == 0;
    }
    
    force_inline void * aligned_malloc(const size_t size, const size_t alignment)
    {
        const size_t padded_size = ( (size - 1) / alignment + 1 ) * alignment;

//            print("---allocation--------------");
//            valprint("size       ",size);
//            valprint("alignment  ",alignment);
//            valprint("padded_size",padded_size);
//            print("---------------------------");
        
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
        void * ptr = _aligned_malloc(padded_size, alignment);
#else
        void * ptr = std::aligned_alloc(alignment, padded_size);
#endif
        
        if( ptr == nullptr )
        {
            eprint("aligned_malloc: failed to allocate memory.");
            return nullptr;
        }
        else
        {
            return ptr;
        }
    }

    force_inline void aligned_free(void * ptr)
    {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            _aligned_free( ptr );
        #else
            free( ptr );
        #endif
    }
    
    template <typename T>
    force_inline int safe_free( T * & ptr )
    {
        int wasallocated = (ptr != nullptr);
        if( wasallocated )
        {
            aligned_free(ptr);
            ptr = nullptr;
            
        }
        return !wasallocated;
    }
    
    template <typename T>
    force_inline int safe_alloc(T * & ptr, size_t size)
    {
        int wasallocated = (ptr != nullptr);
        if( wasallocated )
        {
#ifdef SAFE_ALLOCATE_WARNINGS
            wprint("safe_alloc: Pointer was not NULL. Calling safe_free to prevent memory leak.");
#endif
            safe_free(ptr);
        }
        ptr = static_cast<T*>( aligned_malloc( size * sizeof(T), ALIGNMENT ) );

        return wasallocated;
    }
    
    
    template <typename T>
    force_inline int safe_alloc( T * & ptr, size_t size, T init)
    {
        int wasallocated = safe_alloc(ptr, size);
        
        fill_buffer(ptr, size, init);
        
        return wasallocated;
    }

#if defined(__MINGW64__) || defined(__MINGW32__)  || ( defined(__GNUC__) && (!defined(__clang__)) && (!defined(__APPLE__)) )
// do this *only* for gcc
    
    // overload functions for restrict qualifier
    
    template <typename T>
    force_inline int safe_free( T * restrict & ptr )
    {
        int wasallocated = (ptr != nullptr);
        if( wasallocated )
        {
            aligned_free(ptr); ptr = nullptr;
        }
        return !wasallocated;
    }
    
    template <typename T>
    force_inline int safe_alloc(T * restrict & ptr, size_t size)
    {
        int wasallocated = (ptr != nullptr);
        if( wasallocated )
        {
#ifdef SAFE_ALLOCATE_WARNINGS
            wprint("safe_alloc: Pointer was not NULL. Calling safe_free to prevent memory leak.");
#endif
            safe_free(ptr);
        }
        ptr = static_cast<T *>( aligned_malloc( size * sizeof(T), ALIGNMENT ) );
        
        return wasallocated;
    }
    
    template <typename T>
    force_inline int safe_alloc( T * restrict & ptr, size_t size, T init)
    {
        int wasallocated = safe_alloc(ptr, size);
        
        fill_buffer(ptr, size, init);
        
        return wasallocated;
    }
#endif
    
    template <typename T>
    force_inline void copy_buffer( const T * const from, T * const to, const size_t n )
    {
//        std::memcpy( &to[0], &from[0], n );
        std::copy( &from[0], &from[n], &to[0] );
    }
    
    
    template <typename T>
    force_inline void add_to_buffer( const T * restrict const from, T * restrict const to, const size_t n )
    {
        for( size_t i = 0; i < n; ++i )
        {
            to[i] += from[i];
        }
    }
    
    template <size_t n, typename T>
    force_inline void add_to_buffer( const T * restrict const from, T * restrict const to )
    {
        for( size_t i = 0; i < n; ++i )
        {
            to[i] += from[i];
        }
    }
    
    
    template <typename T>
    force_inline void scale_buffer( const T beta, T * restrict const a, const size_t n )
    {
        for( size_t i = 0; i < n; ++i )
        {
            a[i] *= beta;
        }
    }
    
    template <size_t n, typename T>
    force_inline void scale_buffer( const T beta, T * restrict const a )
    {
        for( size_t i = 0; i < n; ++i )
        {
            a[i] *= beta;
        }
    }
    
    template <typename T>
    force_inline void move_buffer( const T * const from, T * const to, const size_t n )
    {
//        std::memmove( &to[0], &from[0], n );
        std::copy( &from[0], &from[n], &to[0] );
    }
    
    template<typename From, typename To>
    struct static_caster
    {
        To operator()(const From & p)
        {
            return static_cast<To>(p);
        }
    };
    
    template <typename S, typename T>
    force_inline void copy_cast_buffer( const S * const from, T * const to, const size_t n )
    {
        if constexpr ( std::is_same_v<T,S> )
        {
            std::copy( &from[0], &from[n], &to[0] );
        }
        else
        {
            std::transform( &from[0], &from[n], &to[0], static_caster<S,T>() );
        }
    }
    
    template <typename T>
    force_inline void zerofy_buffer( T * const a, const size_t n )
    {
//        std::memset( a, 0, n * sizeof(T) );
        std::fill( &a[0], &a[n], static_cast<T>(0) );
    }
    
    template <typename T, typename S>
    force_inline void fill_buffer( T * const a, const size_t n, const S init )
    {
        std::fill( &a[0], &a[n], static_cast<T>(init) );
    }
    
    template<size_t length, int readwrite, int locality, typename T>
    force_inline void prefetch_range( const T * restrict const begin )
    {
        constexpr size_t PREFETCH_SIZE = length * sizeof(T);
        
        const char * ptr = ((const char*)begin);
    
        LOOP_UNROLL_FULL
        for( size_t offset = 0; offset < PREFETCH_SIZE; offset += PREFETCH_STRIDE )
        {
            prefetch( &ptr[offset], readwrite, locality );
        }
    }
    
    template<int readwrite, int locality, typename T>
    force_inline void prefetch_range( const T * restrict const begin, const size_t length )
    {
        const size_t prefetch_size = length * sizeof(T);
        
        const char * ptr = ((const char*)begin);
    
        for( size_t offset = 0; offset < prefetch_size; offset += PREFETCH_STRIDE )
        {
            prefetch( &ptr[offset], readwrite, locality );
        }
    }
    
    
//    // These don't seem to improve anything.
//    template<int n, typename T>
//    inline void copy_buffer( const T * restrict const from, T * restrict const to)
//    {
//        // With the hope that this can be vectorized.
//        for( long long i = 0; i < n; ++i )
//        {
//            to[i] = from[i];
//        }
//    }
//
//    template<int n, typename T>
//    inline void zerofy_buffer( T * restrict const a )
//    {
//        // With the hope that this can be vectorized.
//        for( long long i = 0; i < n; ++i )
//        {
//            a[i] = static_cast<T>(0);
//        }
//    }
//
//    template<int n, typename T>
//    inline void fill_buffer( T * restrict const a, const T init_ )
//    {
//        const T init = static_cast<T>(init_);
//
//        // With the hope that this can be vectorized.
//        for( long long i = 0; i < n; ++i )
//        {
//            a[i] = init;
//        }
//    }
    
} // namespace Tools
