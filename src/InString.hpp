#pragma once

namespace Tools
{
    struct FromCharResult
    {
        const char * ptr = nullptr;
        bool failedQ = false;
    };
    
    template<typename T>
    struct FromChars
    {
        static constexpr bool implementedQ = false;
    };
    
    // Fix for older versions from https://www.cppstories.com/2019/07/detect-overload-from-chars/
    
    // In herit from false_type per default.
    template <typename T, typename = void>
    struct is_from_chars_convertible : std::false_type {};
    
    // SFINAE: if lookup is successful, inherit from true_type.
    template <typename T>
    struct is_from_chars_convertible<
        T,
        std::void_t<decltype(
            std::from_chars(
                std::declval<const char*>(),
                std::declval<const char*>(),
                declval<T&>()
            )
        )>
    > : std::true_type {};
    
    // Making requests less awkward
    template< class T> inline constexpr bool from_chars_availableQ = is_from_chars_convertible<T>::value;
    

    template<IntQ T>
    struct FromChars<T>
    {
        static constexpr bool implementedQ = true;
        
        FromCharResult operator()( const char * const begin, const char * const end, T & x ) const
        {
            auto r = std::from_chars( begin, end, x, 10 );
            return FromCharResult{ .ptr = r.ptr, .failedQ = (r.ec != std::errc{})};
        }
    };
    
    template<std::floating_point T>
    struct FromChars<T>
    {
        static constexpr bool implementedQ = true;
        
        FromCharResult operator()( const char * const begin, const char * const end, T & x ) const
        {
            
            if constexpr ( from_chars_availableQ<T> )
            {
                auto r = std::from_chars( begin, end, x, std::chars_format::general );
                return FromCharResult{.ptr = r.ptr, .failedQ = (r.ec != std::errc{})};
            }
            else
            {
                // Dirty hack: create a new std::string that internally creates a zero-terminated string. Then apply std::stod.
                
                std::string s (begin, end);
                T value = 0;
                Size_T length = 0;
                bool failedQ = false;
                
                try
                {
                    if constexpr (std::is_same_v<T,float>)
                    {
                        value = std::stof(s, &length);
                    }
                    else if constexpr (std::is_same_v<T,double>)
                    {
                        value = std::stod(s, &length);
                    }
                    else if constexpr (std::is_same_v<T,long double>)
                    {
                        value = std::stold(s, &length);
                    }
                    else
                    {
                        failedQ = true;
                    }
                }
                catch (...)
                {
                    failedQ = true;
                }
                
                // We change the value of x only if no issues occurred, to emulate the behavior of std::from_chars.
                if( !failedQ ) { x = value; }
                
                return FromCharResult{.ptr = &begin[length], .failedQ = failedQ};
            }
        }
    };
    
    
    class InString
    {
        
    public:

        InString() = default;
        
        InString( const char * const begin_, const char * const end_ )
        :   begin { begin_ }
        ,   ptr   { begin_ }
        ,   end   { end_   }
        {}
        
        InString( const char * const begin_, Size_T size )
        :   InString{ begin_, &begin_[size] }
        {}
        
        explicit InString( std::string_view s )
        :   InString{ &*s.begin(), &*s.end() }
        {}

        explicit InString( cref<std::string> s )
        :   InString{ &*s.begin(), &*s.end() }
        {}
        
        explicit InString( std::string && s )
        :   buffer { std::move(s)     }
        ,   begin  { &*buffer.begin() }
        ,   ptr    { &*buffer.begin() }
        ,   end    { &*buffer.end()   }
        {}
        
        explicit InString ( cref<std::filesystem::path> file )
        {
            std::ifstream stream (file, std::ios::in | std::ios::binary);
            if( !stream )
            {
                eprint(ClassName() + "(std::filesystem::path): Opening file " + file.string() + " failed. Returning empty InString.");
                return;
            }
            
            // Obtain the size of the file.
            const auto file_size = std::filesystem::file_size(file);
            
            // Create a buffer.
            buffer = std::string(file_size, '\0');
            // Read the whole file into the buffer.
            stream.read(buffer.data(), static_cast<std::streamsize>(file_size));

            begin = &*buffer.begin();
            ptr   = &*buffer.begin();
            end   = &*buffer.end();
            
            if( FailedQ() )
            {
                eprint(ClassName() + "(std::filesystem::path): Reading from file " + file.string() + " failed.");
                return;
            }
        }
        
        ~InString() = default;
        
    private:
        
        std::string buffer;
        const char * begin  = nullptr;
        const char * ptr    = nullptr;
        const char * end    = nullptr;
        bool failedQ        = false;
        
    public:
        
        Size_T Size() const
        {
            return static_cast<Size_T>(std::distance(ptr,end));
        }
        
        Size_T Capacity() const
        {
            return static_cast<Size_T>(std::distance(begin,end));
        }
        
        bool FailedQ() const
        {
            return failedQ;
        }
        
        void SetFailedQ()
        {
            failedQ = true;
        }
        
        const char & operator[]( const Size_T i ) const
        {
            return ptr[i];
        }
        
        Size_T LineCount() const
        {
            Size_T line_count = 1;
            for( const char * c = ptr; c < end; ++c )
            {
                line_count += (*c == '\n');
            }
            return line_count;
        }
        
#include "InString/Take.hpp"
#include "InString/TakeMatrix.hpp"

//        static InString FromFile( cref<std::filesystem::path> file )
//        {
//            std::ifstream stream (file, std::ios::in | std::ios::binary);
//            if( !stream )
//            {
//                eprint(ClassName() + "(std::filesystem::path): Opening file " + file.string() + " failed. Returning empty InString.");
//                return InString();
//            }
//            
//            // Obtain the size of the file.
//            const auto file_size = std::filesystem::file_size(file);
//            
//            // Create a buffer.
//            std::string s = std::string(file_size, '\0');
//            // Read the whole file into the buffer.
//            stream.read(s.data(), static_cast<std::streamsize>(file_size));
//
//            return InString(std::move(s));
//        }

        
        template<bool checkQ = true>
        InString & Skip( const Size_T n )
        {
            ptr = std::min(ptr + n, end);   
            return *this;
        }
        
        InString & Pop()
        {
            if( ptr > begin )
            {
                --ptr;
            }
            else
            {
                eprint(MethodName("Pop") + ": String buffer is empty. Doing nothing.");
            }
            
            return *this;
        }
        
        InString & Pop( Size_T n )
        {
            if( ptr >= begin + n )
            {
                ptr -= n;
            }
            else
            {
                ptr = begin;
                eprint(MethodName("Pop") + ": Buffer size was smaller than n = " + ToString(n) + ". Emptying it completely.");
            }
            
            return *this;
        }
        
        bool EmptyQ() const
        {
            return ptr >= end;
        }
        
        char CurrentChar() const
        {
            return *ptr;
        }
        
        std::string_view View()
        {
            return std::string_view(ptr,end);
        }
        
    public:
        
        static std::string MethodName( const std::string & tag )
        {
            return ClassName() + "::" + tag;
        }
        
        static std::string ClassName()
        {
            return "InString";
        }
    };
    
} // Tools


