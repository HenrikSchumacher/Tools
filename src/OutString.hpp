#pragma once

#include "OutString/ToChars.hpp"
#include "OutString/CharCount.hpp"
#include "OutString/CharArray.hpp"

namespace Tools
{
    class OutString
    {
    public:
        
        enum class MatrixFormat: UInt8
        {
            Tall   = 0,
            Wide   = 1,
            TSV    = 2,
            CSV    = 3
        };
        
        enum class CubeFormat: UInt8
        {
            Tall   = 0,
            Medium = 1,
            Wide   = 2
        };
        
        template<NonPointerQ EntryFun_T>
        using Result_T = typename std::remove_reference<typename function_traits<EntryFun_T>::return_type>::type;
        
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
        
        template<typename ...Args>
        OutString( std::string && s, Args... args )
        :   OutString( &s[0], s.size() )
        {
            PutChars(std::forward<Args>(args)...);
        }
        
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
        
        template<typename T, typename ToChars_T = ToChars<T>>
        constexpr bool TryEmplace( const T & x, ToChars_T && to_chars )
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

        template<
            NonPointerQ EntryFun_T, NonIntQ ToChars_T, IntQ Int,
            NonIntQ Prefix_T, typename ...Args
        >
        OutString(
            EntryFun_T && a, ToChars_T && to_chars, Int n, Prefix_T && prefix, Args&&... args
        )
        :  OutString{ ArrayCharCount(
            std::forward<ToChars_T>(to_chars), n,
            std::forward<Prefix_T>(prefix), std::forward<Args>(args)...
           ) }
        {
            PutArray(
                std::forward<EntryFun_T>(a),
                std::forward<ToChars_T>(to_chars), false, n,
                std::forward<Prefix_T>(prefix), std::forward<Args>(args)...
            );
        }
        
        template<NonPointerQ EntryFun_T, IntQ Int, NonIntQ Prefix_T, typename ...Args>
        OutString( EntryFun_T && a, Int n, Prefix_T && prefix, Args&&... args )
        :   OutString{
                std::forward<EntryFun_T>(a), ToChars<Result_T<EntryFun_T>>(), n,
                std::forward<Prefix_T>(prefix), std::forward<Args>(args)...
            }
        {}
        
        
        

        template< typename T, IntQ Int_0, NonIntQ ToChars_T = ToChars<T> >
        OutString( cptr<T> a, Int_0 d_0, ToChars_T && to_chars = ToChars_T() )
        {
            PutVector( a, d_0, std::forward<ToChars_T>(to_chars) );
        }
        
        template<
            NonPointerQ EntryFun_T, IntQ Int_0, NonIntQ ToChars_T = ToChars<Result_T<EntryFun_T>>
        >
        OutString( EntryFun_T && a, Int_0 d_0, ToChars_T && to_chars = ToChars_T() )
        {
            PutVectorFun(std::forward<EntryFun_T>(a), d_0, std::forward<ToChars_T>(to_chars));
        }
        
        
        
        
        template<
            MatrixFormat format = MatrixFormat::Tall,
            typename T, IntQ Int_0, IntQ Int_1,
            NonIntQ ToChars_T = ToChars<T>
        >
        OutString(
            cptr<T> a, Int_0 d_0, Int_1 d_1, ToChars_T && to_chars = ToChars_T()
        )
        {
            PutMatrix<format>( a, d_0, d_1, std::forward<ToChars_T>(to_chars) );
        }
        
        template<
            MatrixFormat format = MatrixFormat::Tall,
            NonPointerQ EntryFun_T, IntQ Int_0, IntQ Int_1,
            NonIntQ ToChars_T = ToChars<Result_T<EntryFun_T>>
        >
        OutString(
            EntryFun_T && a, Int_0 d_0, Int_1 d_1, ToChars_T && to_chars = ToChars_T()
        )
        {
            PutMatrixFun<format>(std::forward<EntryFun_T>(a), d_0, d_1, std::forward<ToChars_T>(to_chars));
        }
        
        
        
        template<
            CubeFormat format = CubeFormat::Medium,
            typename T, IntQ Int_0, IntQ Int_1, IntQ Int_2,
            NonIntQ ToChars_T = ToChars<T>
        >
        OutString(
            cptr<T> a, Int_0 d_0, Int_1 d_1, Int_2 d_2, ToChars_T && to_chars = ToChars_T()
        )
        {
            PutCube<format>(
                a, d_0, d_1, d_2, std::forward<ToChars_T>(to_chars)
            );
        }
        
        template<
            CubeFormat format = CubeFormat::Medium,
            NonPointerQ EntryFun_T, IntQ Int_0, IntQ Int_1, IntQ Int_2,
            NonIntQ ToChars_T = ToChars<Result_T<EntryFun_T>>
        >
        OutString(
            EntryFun_T && a, Int_0 d_0, Int_1 d_1, Int_2 d_2, ToChars_T && to_chars = ToChars_T()
        )
        {
            PutCubeFun<format>(
                std::forward<EntryFun_T>(a), d_0, d_1, d_2, std::forward<ToChars_T>(to_chars)
            );
        }
        
        
        
//        CubeFormat format = CubeFormat::Medium,
        
        
        
//        std::string_view View() const
//        {
//            return std::string_view(first,ptr);
//        }
//        
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

} // Tools


