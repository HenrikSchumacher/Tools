#pragma once

#include "OutString/ToChars.hpp"
#include "OutString/CharCount.hpp"
#include "OutString/CharArray.hpp"

namespace Tools
{
    
//    template<typename F, typename T>
//    concept CharConv = std::invocable<F,T> && SameQ<std::invoke_result_t<F,T>,ToCharResult>;
    
//    template<typename F, typename T>
//    concept CharConv = std::invocable<F,T> && SameQ<std::invoke_result_t<F,T>,ToCharResult>;
    
//    template<typename F, typename T>
//    concept CharConv = SameQ<std::invoke_result_t<F, char *, char *, T&>,ToCharResult>;
  
//    template<typename C, typename T>
//    concept CharConv = std::invocable<C,char*, const char*, const T&>;
//    template<typename C, typename T>
//    concept CharConv = NonPointerQ<C> && NonIntQ<C>;
    
    template<typename C, typename T>
    concept CharConv = NonIntQ<C>;
    
//    template<typename F, typename ...Int>
//    concept ArrayFun = std::invocable<F,Int...>;
    
//    template<typename A, typename ...Int>
//    concept ArrayFun = NonPointerQ<A> && NonIntQ<A>;
    
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
    

    
    class OutString
    {
    public:
        
        template<typename A>
        using Result_T = typename std::remove_reference<typename function_traits<A>::return_type>::type;
    
        
//        template<typename A, typename ...Int>
////        requires ArrayFun<A,Int...>
//        using Result_T = typename std::remove_reference<typename function_traits<A>::return_type>::type;
//        
        
//        template<typename A, typename ...Int>
//        requires ArrayFun<A,Int...>
//        using Result_T = typename std::remove_reference<typename std::invoke_result_t<A,Int...>>;

        
        
        
    public:

        OutString()
        :   OutString { Size_T(16) }
        {}
        
        OutString( Size_T n )
        {
            safe_alloc(first,n);
            ptr = first;
            last = &first[n];
        }

        OutString( const char * c, Size_T n )
        {
            safe_alloc(first,n);
            last = &first[n];
            std::copy_n(c,n,first);
            ptr = last;
        }
        
        OutString( const std::string_view s )
        :   OutString( &s[0], s.size() )
        {}
        
//        OutString( std::string && s )
//        {
//            safe_alloc(first,s.size());
//            last = &first[s.size()];
//            std::copy_n(&s[0],s.size(),first);
//            ptr = last;
//        }
//        
//        template<typename ...Args>
//        OutString( std::string_view && s, Args... args )
//        :   OutString( &s[0], s.size() )
//        {
//            PutChars(std::forward<Args>(args)...);
//        }
        
        OutString( Size_T n, const char & x )
        :   OutString { Size_T(n) }
        {
            std::fill(first,last,x);
            ptr = last;
        }
    
        
        ~OutString() noexcept
        {
            safe_free(first);
        }
        
        
        // Copy constructor
        OutString( const OutString & other ) noexcept
        :   OutString( &other[0], other.Capacity() )
        {
            std::copy( other.first, other.last, first);
            last = first + other.Capacity();
            ptr = first + other.Size();
        }

        inline friend void swap( OutString & A, OutString & B ) noexcept
        {
            using std::swap;
            
            if( &A != &B )
            {
                swap( A.first, B.first );
                swap( A.ptr  , B.ptr   );
                swap( A.last , B.last  );
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
        
        char * first = nullptr; // This is the owning pointer.
        char * last  = nullptr;
        char * ptr   = nullptr;
        
    public:
        
        Size_T Size() const
        {
            return static_cast<Size_T>(std::distance(first,ptr));
        }
        
        Size_T Capacity() const
        {
            return static_cast<Size_T>(std::distance(first,last));
        }
        
        const char * begin() const
        {
            return first;
        }
        
        const char * end() const
        {
            return last;
        }
        
        char & operator[]( const Size_T i )
        {
            return first[i];
        }
        
        const char & operator[]( const Size_T i ) const
        {
            return first[i];
        }
        
#include "OutString/Put.hpp"
#include "OutString/PutVector.hpp"
#include "OutString/PutMatrix.hpp"
#include "OutString/PutCube.hpp"
#include "OutString/PutArray.hpp"

    public:
        
        template<bool checkQ = true>
        OutString & Skip( const Size_T n )
        {
            if constexpr ( checkQ )
            {
                RequireFreeSpace( Size_T(n) );
            }
            ptr += n;
            
            return *this;
        }
        
        OutString & Pop()
        {
            if( ptr > first )
            {
                --ptr;
            }
            else
            {
                eprint(MethodName("Pop") + ": String buffer is empty. Doing nothing.");
            }
            
            return *this;
        }
        
        OutString & Pop( Size_T n )
        {
            if( ptr >= first + n )
            {
                ptr -= n;
            }
            else
            {
                ptr = first;
                eprint(MethodName("Pop") + ": Buffer size was smaller than n = " + ToString(n) + ". Emptying it completely.");
            }
            
            return *this;
        }
        
        OutString & Clear()
        {
            ptr = first;
            return *this;
        }
        
        bool EmptyQ() const
        {
            return ptr == first;
        }
        
        bool FullQ() const
        {
            return ptr == last;
        }
        
        std::ptrdiff_t FreeSpace() const
        {
            return std::distance(ptr,last);
        }
        
        OutString & RequireFreeSpace( Size_T n )
        {
            if( &ptr[n] > last )
            {
                Resize(Size_T(2) * Capacity() + static_cast<Size_T>(std::distance(last,&ptr[n])));
            }
            return *this;
        }
        
    
        
        OutString & Expand()
        {
            Resize( Size_T(2) * Capacity() );
            return *this;
        }
        
        OutString & Resize( const Size_T & n )
        {
//            print("Resize");
            char * s = nullptr;
            Size_T capacity = std::max(Capacity(),n);
            Size_T size     = Size();
            safe_alloc(s,capacity);
            std::copy(first,ptr,s);
            safe_free(first);
            first = s;
            last = &first[capacity];
            ptr = &first[size];
            
            return *this;
        }
        
        char & Front()
        {
            return *first;
        }
        
        const char & Front() const
        {
            return *first;
        }
        
        char & Back()
        {
            return *ptr;
        }
        
        const char & Back() const
        {
            return *ptr;
        }
        
    private:

        constexpr bool TryEmplaceChar( const char & c )
        {
            if( ptr >= last )
            {
                return true;
            }
            else
            {
                *ptr = c;
                return false;
            }
        }
        
        template<typename T, CharConv<T> C = ToChars<T>>
        constexpr bool TryEmplace( const T & x, C && to_chars )
        {
            auto r = to_chars(ptr, last, x);
            if( r.failedQ )
            {
                return true;
            }
            else
            {
                ptr = r.ptr;
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
        
//        std::string_view View() const
//        {
//            return std::string_view(first,ptr);
//        }

        operator std::string_view () const
        {
            return std::string_view(first,ptr);
        }
        
        operator std::string () const
        {
            return std::string(first,ptr);
        }
        
        friend std::ostream & operator<<( std::ostream & out, const OutString & in )
        {
            return (out << std::string_view(in));
        }
        
//        friend std::ofstream & operator<<( std::ofstream & out, OutString && in )
//        {
//            return (out << in.View());
//        }
        
    public:
        
        static std::string MethodName( const std::string & tag )
        {
            return ClassName() + "::" + tag;
        }
        
        static std::string ClassName()
        {
            return "OutString";
        }
        
    }; // OutString

    
    
    
    template<typename T, NonIntQ F>
    [[nodiscard]] OutString ToString( cref<std::vector<T>> v )
    {
        return OutString::FromVector( &v[0], v.size() );
    }
    
    template<typename T, Size_T N, NonIntQ F>
    [[nodiscard]] OutString ToString( const std::array<T,N> & v )
    {
        return OutString::FromVector( &v[0], N );
    }
    
    
} // Tools


