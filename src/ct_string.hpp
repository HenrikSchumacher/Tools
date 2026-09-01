namespace Tools
{

    
    template<Size_T N>
    class ct_string
    {
        // Taken from https://stackoverflow.com/a/77803192/8248900
        
        static_assert( N > Size_T{0} );
        
        static constexpr Size_T n = N - Size_T{1};
        
    private:
        
        char bytes[N];
        
    public:
        
        consteval ct_string()
        {
            for( Size_T i = 0; i < n; ++i )
            {
                bytes[i] = ' ';
            }
            bytes[n] = '\0';
        }
//        
        consteval ct_string( const ct_string & rhs )
        {
            for( Size_T i = Size_T{0}; i < N; ++i )
            {
                bytes[i] = rhs[i];
            }
        }
        
        // Construct from a "string literal":
        consteval ct_string( const char(&arr)[N] )
        {
            for( Size_T i = 0; i < N; ++i ) { bytes[i] = arr[i]; }
        }
        
        // does not include trailing nil
        // so ct_string<10> has a max size of 9
        // if there is an earlier nil character, size()
        // is the length up to that nil
        constexpr Size_T strlen() const
        {
            for( Size_T r = 0; r < n; ++r )
            {
                if( bytes[r] == '\0' ) { return r; }
            }
            return n;
        }
        
        consteval Size_T size() const
        {
            return n;
        }
        
        consteval Size_T byte_count() const { return N; }
        
        template<IntQ Int>
        constexpr char & operator[](const Int i)
        {
            return bytes[i];
        }
        
        template<IntQ Int>
        constexpr char const & operator[](const Int i) const
        {
            return bytes[i];
        }
        
        constexpr char const * data() const
        {
            return bytes;
        }
    
        constexpr char * begin()
        {
            return &bytes[0];
        }
    
        constexpr char const * begin() const
        {
            return &bytes[0];
        }
    
        constexpr char * end()
        {
            return &bytes[n];
        }

        constexpr char const * end() const
        {
            return &bytes[n];
        }
        
        constexpr operator std::string_view () const
        {
            return std::string_view(&bytes[0],&bytes[n]);
        }
        
        friend std::ostream & operator<<( std::ostream & stream, const ct_string & s )
        {
            return (stream << std::string_view(s));
        }
        
        friend std::string ToString( const ct_string & s )
        {
            return std::string(&s.bytes[0],n);
        }
        
//
//        operator std::string () const
//        {
//            return std::string(&bytes[0],n);
//        }
//        
//        constexpr operator char const*() const
//        {
//            return data();
//        }
        
//        constexpr const char * c_str() const
//        {
//            char b [N];
//            for( Size_T i = 0; i < N; ++i ) { b[i] = bytes[i]; }
//            return b;
//        }
    };
    
    
    template<Size_T M, Size_T N>
    [[nodiscard]] consteval ct_string<M+N-Size_T{1}> operator+(
        const ct_string<M> & lhs, const ct_string<N> & rhs
    )
    {
        ct_string<M+N-Size_T{1}> result;
        
        // copy up to first nil in lhs:
        for (Size_T i = 0; i < lhs.size(); ++i)
        {
            result[i] = lhs[i];
        }
        // copy entire rhs buffer, including trailing nils:
        for (Size_T i = 0; i < rhs.size(); ++i)
        {
            result[lhs.size()+i] = rhs[i];
        }
//        
//        // zero out the leftovers, if any:
//        for( Size_T i = lhs.size() + N; i < M+N-1; ++i )
//        {
//            result[i] = 0;
//        }
        
        return result;
    }
    
    
    template<Size_T M, Size_T N>
    [[nodiscard]] consteval ct_string<M+N-Size_T{1}> operator+(
        const char(&lhs)[M], const ct_string<N> & rhs
    )
    {
        return ct_string<M>(lhs) + rhs;
    }
    
    template<Size_T M, Size_T N>
    [[nodiscard]] consteval ct_string<M+N-Size_T{1}> operator+(
        const ct_string<M> & lhs, const char(&rhs)[N]
    )
    {
        return lhs + ct_string<N>(rhs);
    }
    
    // constexpr string concatenation is a C++20 feature. Older compilers might not support it.
    template<Size_T N>
    [[nodiscard]] constexpr std::string operator+(
        const ct_string<N> & lhs, const std::string & rhs
    )
    {
        return lhs.data() + rhs;
    }
    
    // constexpr string concatenation is a C++20 feature. Older compilers might not support it.
    template<Size_T N>
    [[nodiscard]] constexpr std::string operator+(
        const std::string & lhs, const ct_string<N> & rhs
    )
    {
        return lhs + rhs.data();
    }
    
    template<Size_T t >
    constexpr ct_string<t + 1> ct_tabs = ct_tabs<t-1> + ct_string<2>("\t");
    template<> constexpr ct_string<1> ct_tabs<0> = ct_string<1>("");
    
    template<Size_T t >
    constexpr ct_string<t + 1> ct_spaces = ct_spaces<t-1> + ct_string<2>(" ");
    template<> constexpr ct_string<1> ct_spaces<0> = ct_string<1>("");
    
    
    consteval ct_string<2> to_ct_string( const bool b )
    {
        return b ? "1" : "0";
    }

    // Slows down everything?
    template<IntQ Int>
    consteval ct_string<std::numeric_limits<Int>::digits10+3>
    to_ct_string( const Int i )
    {
        using Int8 = std::int8_t;
        
        constexpr Size_T N = std::numeric_limits<Int>::digits10+3;

        constexpr char digits [11] = "0123456789";

        Int8 x [11];

        ct_string<N> s;

        Size_T x_ptr = 0;

        Int d = i % Int{10};
        Int r = i / Int{10};

        x[x_ptr++] = static_cast<Int8>(d);

        while( r != 0 )
        {
            d = r % Int{10};
            r = r / Int{10};

            x[x_ptr++] = static_cast<Int8>(d);
        }

        Int s_ptr = 0;

        if( i < Int{0} )
        {
            s[s_ptr++] = '-';
        }

        // Reverse
        while( x_ptr > Int{0} )
        {
            s[s_ptr++] = digits[x[--x_ptr]];
        }

        return s;
    }
} // namespace Tools
