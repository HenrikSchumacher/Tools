#pragma once

#include "OutString/ToChars.hpp"
#include "OutString/CharCount.hpp"
#include "OutString/CharArray.hpp"

namespace Tools
{
    // Not a really good discriminator, but good enough to distinguish a char converter class from an integral type.
    template<typename C, typename T>
    concept CharConv = NonIntQ<C>;
    
    template<typename A, typename ...Int>
    concept ArrayFun = NonPointerQ<A>;
    
    template<class T>
    concept Stringy = std::is_convertible_v<T, std::string_view>;
    
    template<class T>
    concept NonStringy = !Stringy<T>;
    
    namespace Format
    {
        namespace Vector
        {
            struct Default
            {
                static constexpr char prefix_0 [3] = "{ ";
                static constexpr char infix_0  [3] = ", ";
                static constexpr char suffix_0 [3] = " }";
            };
            
            struct Space
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = " ";
                static constexpr char suffix_0 [1] = "";
            };
            
            struct TSV
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = "\t";
                static constexpr char suffix_0 [1] = "";
            };
        }
        
        namespace Matrix
        {
            struct Tall
            {
                static constexpr char prefix_0 [3] = "{\n";
                static constexpr char infix_0  [3] = ",\n";
                static constexpr char suffix_0 [3] = "\n}";
                static constexpr char prefix_1 [4] = " { ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
            };
            
            struct Wide
            {
                static constexpr char prefix_0 [3] = "{ ";
                static constexpr char infix_0  [3] = ", ";
                static constexpr char suffix_0 [3] = " }";
                static constexpr char prefix_1 [4] = "{ ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
            };
            
            struct TSV
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = "\n";
                static constexpr char suffix_0 [1] = "";
                static constexpr char prefix_1 [1] = "";
                static constexpr char infix_1  [2] = "\t";
                static constexpr char suffix_1 [1] = "";
            };
            
            struct CSV
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = "\n";
                static constexpr char suffix_0 [1] = "";
                static constexpr char prefix_1 [1] = "";
                static constexpr char infix_1  [2] = ",";
                static constexpr char suffix_1 [1] = "";
            };
        }
        
        
        namespace Cube
        {
            struct Tall
            {
                static constexpr char prefix_0 [3] = "{\n";
                static constexpr char infix_0  [3] = ",\n";
                static constexpr char suffix_0 [3] = "\n}";
                static constexpr char prefix_1 [4] = " {\n";
                static constexpr char infix_1  [3] = ",\n";
                static constexpr char suffix_1 [4] = "\n }";
                static constexpr char prefix_2 [5] = "  { ";
                static constexpr char infix_2  [3] = ", ";
                static constexpr char suffix_2 [3] = " }";
            };
            
            struct Wide
            {
                static constexpr char prefix_0 [3] = "{ ";
                static constexpr char infix_0  [3] = ", ";
                static constexpr char suffix_0 [3] = " }";
                static constexpr char prefix_1 [3] = "{ ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
                static constexpr char prefix_2 [3] = "{ ";
                static constexpr char infix_2  [3] = ", ";
                static constexpr char suffix_2 [3] = " }";
            };
            
            struct Medium
            {
                static constexpr char prefix_0 [3] = "{\n";
                static constexpr char infix_0  [3] = ",\n";
                static constexpr char suffix_0 [3] = "\n}";
                static constexpr char prefix_1 [4] = " { ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
                static constexpr char prefix_2 [3] = "{ ";
                static constexpr char infix_2  [3] = ", ";
                static constexpr char suffix_2 [3] = " }";
            };
        }
    }
    

    /*!@brief A class to resemble `std::ostringstream`, except being faster for big arrays at the cost of somewhat limited capabilities.
     *
     * This class uses overloads of `ToChars` or interface-compatible type-to-chars converters to do its job.
     * */
    class OutString
    {
        // TODO: Maybe OutString needs better error handling. In particular, we need an internal `failed` flag and a function `bool FailedQ()` that returns it. All operations have to check the flag `failed` first. This does not have super high priority for writing operations since `ToChars` never writes out of bounds. But the allocation routines may fail (and throw). Maybe it would be nice if we could move to non-throw behavior.
        
    public:
        
        using Int = Size_T;
        
        template<typename A>
        using Result_T = typename std::remove_reference<typename function_traits<A>::return_type>::type;
    
    public:

        OutString()
        :   OutString { Int(16) }
        {}
    
        explicit OutString( Int n )
        {
            Allocate(n);
        }

        OutString( const char * c, Int size_, Int capacity_ )
        {
            Allocate(capacity_);
            std::copy_n(c,size_,buffer);
            size = size_;
        }
        
        OutString( const char * c, Int size_ )
        :   OutString( c, size_, size_ )
        {}
        
        OutString( const std::string_view s )
        :   OutString( &s[0], s.size() )
        {}

        OutString( Size_T size_, char  x )
        :   OutString { Size_T(size_) }
        {
            std::fill_n(buffer,size_,x);
        }
        
        ~OutString() noexcept
        {
            Deallocate();
        }
        
        // Copy constructor
        OutString( const OutString & other ) noexcept
        :   OutString( &other[0], other.Size(), other.Capacity() )
        {}

        inline friend void swap( OutString & A, OutString & B ) noexcept
        {
            using std::swap;
            
            if( &A != &B )
            {
                swap( A.buffer  , B.buffer   );
                swap( A.size    , B.size     );
                swap( A.capacity, B.capacity );
            }
        }

        // Copy assignment operator
        OutString & operator=( OutString other ) noexcept
        {
            if( this != &other )
            {
                swap(*this,other);
            }
            return *this;
        }
        
        // Move constructor
        OutString( OutString && other ) noexcept
        :   OutString()
        {
            swap(*this, other);
        }

        // Move assignment operator
        mref<OutString> operator=( OutString && other ) noexcept
        {
            if( this != &other )
            {
                swap(*this, other);
            }
            return *this;
        }
        
    private:
        
        char * buffer = nullptr; // This is the owning pointer.
        Int size      = 0;
        Int capacity  = 0;
        
    private:
        
        void Deallocate()
        {
            if( buffer != nullptr )
            {
                free(buffer);
                buffer   = nullptr;
                size     = 0;
                capacity = 0;
            }
        }
        
        void Allocate( Int size_ )
        {
            Deallocate();

            buffer = (char*)malloc(ToSize_T(size_));

            if( buffer == nullptr )
            {
                std::string msg;
                msg += "OutString(";
                msg += ToString(size_);
                msg += ") failed to allocate memory.";
                throw std::runtime_error(msg);
            }
            
            capacity = size_;
        }
        
    public:
        
        Int Size() const
        {
            return size;
        }
        
        Int Capacity() const
        {
            return capacity;
        }

        char * begin()
        {
            return buffer;
        }
        
        char * ptr()
        {
            return &buffer[size];
        }
        
        char * end()
        {
            return &buffer[capacity];
        }
        
        const char * begin() const
        {
            return buffer;
        }
        
        const char * ptr() const
        {
            return &buffer[size];
        }
        
        const char * end() const
        {
            return &buffer[capacity];
        }
        
        char & operator[]( const Int i )
        {
            return buffer[i];
        }
        
        const char & operator[]( const Int i ) const
        {
            return buffer[i];
        }
        
#include "OutString/Put.hpp"
#include "OutString/PutVector.hpp"
#include "OutString/PutMatrix.hpp"
#include "OutString/PutCube.hpp"
#include "OutString/PutArray.hpp"

    public:
        
        template<bool checkQ = true>
        OutString & Skip( const Int n )
        {
            if constexpr ( checkQ )
            {
                RequireFreeSpace( Int(n) );
            }
            size += n;
            
            return *this;
        }
        
        OutString & Pop()
        {
            if( size > Int(0) )
            {
                --size;
            }
            
            return *this;
        }
        
        OutString & Pop( Int n )
        {
            if( size >= n )
            {
                size -= n;
            }
            else
            {
                size = 0;
            }
            
            return *this;
        }
        
        OutString & Clear()
        {
            size = 0;
            return *this;
        }
        
        bool EmptyQ() const
        {
            return size == Size_T(0);
        }
        
        bool FullQ() const
        {
            return size >= capacity;
        }
        
        Int FreeSpace() const
        {
            return capacity - size;
        }
        
        
    private:
        
        OutString & Resize( const Int & n )
        {
            // We really want to enforce here that the size after calling this function equals `n`.
            
            if( std::cmp_equal(capacity,n) ) { return *this; }
            
            OutString other ( buffer, size, std::max(capacity,n) );

            swap(*this,other);
            return *this;
        }
        
    public:
        
        OutString & RequireCapacity( Int n )
        {
            if( n > capacity )
            {
                Resize(Int(2) * capacity + size + n);
            }
            return *this;
        }
        
        OutString & RequireFreeSpace( Int n )
        {
            if( size + n > capacity )
            {
                Resize( capacity + size + n );
            }
            return *this;
        }
        
        OutString & Expand()
        {
            RequireCapacity( Int(2) * capacity );
            return *this;
        }
        
        char & Front()
        {
            return buffer[0];
        }
        
        const char & Front() const
        {
            return buffer[0];
        }
        
        char & Back()
        {
            return buffer[size];
        }
        
        const char & Back() const
        {
            return buffer[size];
        }
        
    private:

        constexpr bool TryEmplaceChar( const char & c )
        {
            if( size >= capacity )
            {
                return true;
            }
            else
            {
                buffer[size] = c;
                ++size;
                return false;
            }
        }
        
        template<typename T, CharConv<T> C = ToChars<T>>
        constexpr bool TryEmplace( const T & x, C && to_chars )
        {
            char * p = ptr();
            auto r = to_chars(p, end(), x);
            if( r.failedQ )
            {
                return true;
            }
            else
            {
                size += static_cast<Size_T>(std::distance(p,r.ptr));
                return false;
            }
        }
        
        template<typename T>
        constexpr bool TryEmplace( const T & x )
        {
            return TryEmplace(x, ToChars<T>() );
        }
        
    public:

        friend Size_T CharCount( const OutString & s )
        {
            return s.Size();
        }
        
        std::string_view View() const
        {
            return std::string_view(begin(),ptr());
        }

        operator std::string_view () const
        {
            return std::string_view(begin(),ptr());
        }
        
        operator std::string () const
        {
            return std::string(begin(),ptr());
        }
        
        friend std::ostream & operator<<( std::ostream & out, const OutString & in )
        {
            return (out << std::string_view(in));
        }
        
    public:
        
        static constexpr std::string MethodName( const std::string & tag )
        {
            return ClassName() + "::" + tag;
        }
        
        static constexpr std::string ClassName()
        {
            return "OutString";
        }
        
    }; // OutString

    
    
    template<typename T>
    [[nodiscard]] OutString ToString( cref<std::vector<T>> v )
    {
        return OutString::FromVector( &v[0], v.size() );
    }
    
    template<typename T, Size_T N>
    [[nodiscard]] OutString ToString( const std::array<T,N> & v )
    {
        return OutString::FromVector( &v[0], N );
    }
    
    
} // Tools


