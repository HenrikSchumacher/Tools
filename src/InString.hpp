#pragma once


namespace Tools
{
    /*!@brief A class to resemble `std::istringstream`, except being faster for big arrays at the cost of somewhat limited capabilities. Features also some limited parsing capabilities.
     *
     * This class uses overloads of `FromChars` or interface-compatible chars-to-type converters to do its job.
     */
    class InString
    {
        using Int = Size_T;
        
        template<typename A>
        using Result_T = typename std::remove_reference<typename function_traits<A>::return_type>::type;
        
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
            constexpr auto tag = ct_string("InString(cref<std::filesystem::path>)");
            
            std::ifstream stream (file, std::ios::in | std::ios::binary);
            if( !stream )
            {
                Msgr::eprint(tag, "Opening file ", file.string(), " failed. Returning empty InString.");
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
                Msgr::eprint(tag, "Reading from file ", file.string(), " failed.");
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
        
        Size_T Position() const
        {
            return static_cast<Size_T>(std::distance(begin,ptr));
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
                if( *c == '\n' )
                {
                    ++line_count;
                }
                else if( *c == '\r' )
                {
                    ++line_count;
                    
                    if( (c < end) && (c[1] == '\n') )
                    {
                        // CAUTION: We increment the loop variable `c` here.
                        // We have to do that because we must not count `\r\n` as two newlines.
                        ++c;
                    }
                }
            }
            return line_count;
        }
        
#include "InString/Take.hpp"
#include "InString/Skip.hpp"
#include "InString/TakeArray.hpp"
#include "InString/TakeVector.hpp"
#include "InString/TakeMatrix.hpp"
#include "InString/TakeCube.hpp"
        
        InString & Pop()
        {
            if( ptr > begin )
            {
                --ptr;
            }
            else
            {
                Msgr::eprint("Pop", "String buffer is empty. Doing nothing.");
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
                Msgr::eprint("Pop", ": Buffer size was smaller than n = ", n, ". Emptying it completely.");
            }
            
            return *this;
        }

    public:
        
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
        
        using Msgr = Tools::Messenger<InString>;
        
        static consteval auto ClassName()
        {
            return ct_string("InString");
        }
    };
    
} // Tools


