#pragma once

namespace Tools
{
    template<typename T>
    class AlignedAllocator()
    {
    public:

        force_inline static int Free( T * & ptr)
        {
            const int wasallocated = (ptr != nullptr);
            
            if( wasallocated )
            {
                aligned_free(ptr);
                ptr = nullptr;

            }
            
            return !wasallocated;
        }
        
#if COMPILER_IS_ANAL_ABOUT_RESTRICT
        // overload function for restrict qualifier
        template <typename T>
        force_inline int safe_free( T * restrict & ptr )
        {
            const int wasallocated = (ptr != nullptr);
            
            if( wasallocated )
            {
                aligned_free(ptr_);
                ptr = nullptr;
            }
            
            return !wasallocated;
        }
#endif
        
        
        force_inline static int Allocate(
            T * & ptr, const Size_T n, const Size_T aligment = Alignment
        )
        {
            int wasallocated = (ptr != nullptr);

            if( wasallocated != 0 ) [[unlikely]]
            {
                wprint(ClassName() + ":: Allocate: Pointer was not nullptr. Calling Free to prevent memory leak.");
                Free(ptr);
            }

            ptr_ = static_cast<T *>( aligned_malloc( n * sizeof(T), aligment ) );

            return wasallocated;
        }
        
        
#if COMPILER_IS_ANAL_ABOUT_RESTRICT
        // overload function for restrict qualifier
        template <typename T>
        force_inline int Allocate( T * restrict & ptr_, const Size_T n, const Size_T aligment = Alignment )
        {
            int wasallocated = (ptr_ != nullptr);

            if( wasallocated != 0 ) [[unlikely]]
            {
                wprint(ClassName() + ":: Allocate: Pointer was not nullptr. Calling Free to prevent memory leak.");
                Free(ptr_);
            }

            ptr_ = static_cast<T *>( aligned_malloc( n * sizeof(T), aligment ) );

            return wasallocated;
        }
#endif
        
        static std::string ClassName()
        {
            return std::string("AlignedAllocator")+"<" + TypeName<T> + ">";
        }
    }
    
} // namespace Tools
