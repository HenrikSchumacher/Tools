#pragma once

namespace Tools
{
    template<typename T, bool verboseQ = false>
    class AlignedAllocator
    {
    public:
        
        force_inline static int Free( T * & ptr)
        {
            const int wasallocated = (ptr != nullptr);
            
            if( wasallocated )
            {
                aligned_free(ptr);
                ptr = nullptr;
                
                if constexpr ( verboseQ )
                {
                    print(ClassName() + "::Alloc: Memory at " + ToString(ptr) + " has been released." );
                }
            }
            
            return !wasallocated;
        }
        
#if COMPILER_IS_ANAL_ABOUT_RESTRICT
        // overload function for restrict qualifier
        force_inline static int Free( T * restrict & ptr )
        {
            const int wasallocated = (ptr != nullptr);
            
            if( wasallocated )
            {
                aligned_free(ptr);
                ptr = nullptr;
                
                if constexpr ( verboseQ )
                {
                    print(ClassName() + "::Alloc: Memory at " + ToString(ptr) + " has been released." );
                }
            }
            
            return !wasallocated;
        }
#endif
        
        force_inline static int Alloc(
            T * & ptr, const Size_T n, const Size_T aligment = DefaultAlignment
        )
        {
            int wasallocated = (ptr != nullptr);
            
            if( wasallocated != 0 ) [[unlikely]]
            {
                wprint(ClassName() + "::Alloc: Pointer was not nullptr. Calling Free to prevent memory leak.");
                Free(ptr);
            }
            
            ptr = static_cast<T *>( aligned_malloc( n * sizeof(T), aligment ) );
            
            if( ptr == nullptr ) [[unlikely]]
            {
                eprint(ClassName() + "::Alloc: Allocation of array of size " + ToString(n) + " failed." );
            }
            else
            {
                if constexpr ( verboseQ )
                {
                    print(ClassName() + "::Alloc: Allocation of array of size " + ToString(n) + " at " + ToString(ptr) + " succeeded." );
                }
            }
            
            return wasallocated;
        }
        
        
#if COMPILER_IS_ANAL_ABOUT_RESTRICT
        // overload function for restrict qualifier
        force_inline static int Alloc(
            T * restrict & ptr, const Size_T n, const Size_T aligment = DefaultAlignment
        )
        {
            int wasallocated = (ptr != nullptr);
            
            if( wasallocated != 0 ) [[unlikely]]
            {
                wprint(ClassName() + ":: Alloc: Pointer was not nullptr. Calling Free to prevent memory leak.");
                Free(ptr);
            }
            
            ptr = static_cast<T *>( aligned_malloc( n * sizeof(T), aligment ) );
            
            if( ptr == nullptr ) [[unlikely]]
            {
                eprint(ClassName() + "::Alloc: Allocation of array of size " + ToString(n) + " failed." );
            }
            else
            {
                if constexpr ( verboseQ )
                {
                    print(ClassName() + "::Alloc: Allocation of array of size " + ToString(n) + " at " + ToString(ptr) + " succeeded." );
                }
            }
            
            return wasallocated;
        }
#endif
        
        static std::string ClassName()
        {
            return std::string("AlignedAllocator")+"<" + TypeName<T> + "," + ToString(verboseQ) + ">";
        }
        
    }; // class AlignedAllocator
    
} // namespace Tools
