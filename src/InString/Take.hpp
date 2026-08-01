/*!@brief Skips arbitarily many whitespace characters until it finds a non-whitspace character or the end of the buffer. Does _not_ fail if no such whitespace is found.*/
InString & SkipWhiteSpace()
{
    if( failedQ ) return *this;
    
    while( !EmptyQ() && std::isspace(*ptr) ) { ++ptr; }
    
    return *this;
}

/*!@brief Check whether `CurrentChar()` equals `c`. If succeeded, skip this character; otherwise (or if the buffer end is hit) fail.*/
InString & SkipChar( const char & c )
{
    if( failedQ ) return *this;
    
    if( !EmptyQ() && (c == *ptr) )
    {
        ++ptr;
    }
    else
    {
        failedQ = true;
    }
    return *this;
}

/*!@brief Check whether there are at least`n` characters left in the buffer and whether the next `n` characters equal the character sequence in `c`. */
constexpr bool StartsWithQ( const char * c, const Size_T n )
{
    return (ptr + n <= end) && (std::strncmp(c, ptr, n) == 0);
}

/*!@brief Check whether there are at least`n` characters left in the buffer and whether the next `n` characters equal the character sequence in `c`. */
template<Size_T n>
constexpr bool StartsWithQ( const char(&c)[n] )
{
    return StartsWithQ( &c[0], n-1 );
}

/*!@brief Check whether there are at least`n` characters left in the buffer and whether the next `n` characters equal the character sequence in `c`. If succeeded, it skips thes characters; otherwise (or if the buffer end is hit), it fail.*/
constexpr InString & SkipChars( const char * c, const Size_T n )
{
    if( failedQ ) return *this;
    
    if( StartsWithQ(c,n) )
    {
        ptr += n;
    }
    else
    {
        failedQ = true;
    }
    return *this;
}

/*!@brief Check whether there are at least`n` characters left in the buffer and whether the next `n` characters equal the character sequence in `c`. If succeeded, it skips thes characters; otherwise (or if the buffer end is hit), it fail.*/
template<Size_T n>
constexpr InString & SkipChars( const char(&c)[n] )
{
    return SkipChars( &c[0], n-1 );
}

/*!@brief Attempt to read the a object of type `T` from the buffer and store it in reference `x`.*/
template<typename T, typename FromChars_T = FromChars<T>>
InString & Take( T & x, FromChars_T && from_chars = FromChars_T() )
{
    if( failedQ ) return *this;
    
    auto r = from_chars( ptr, end, x );
    
    if( r.failedQ )
    {
        failedQ = true;
    }
    else
    {
        ptr = r.ptr;
    }
    return *this;
}

/*!@brief Checks whether currect buffer stars with a newline character sequence.r
 *
 * We support these syntaxes: `'\n'` (Unix world), `\r\n` (Windows),`'\r'` (old mac OSes; some dated versions of Excel). `\n\r` is interpreted as two newlines.
 */
bool NewlineQ()
{
    return (*ptr == '\n') || (*ptr == '\r');
}

/*!@brief Skip a newline symbol or symbol sequence; fails if no such sequence is found.
 *
 * We support these syntaxes: `'\n'` (Unix world), `\r\n` (Windows),`'\r'` (old mac OSes; some dated versions of Excel). `\n\r` is interpreted as two newlines.
 */
InString & SkipNewline()
{
    if( failedQ ) return *this;
    
    if( !EmptyQ() )
    {
        failedQ = true;
        return *this;
    }
    
    if( *ptr == '\n' )
    {
        ++ptr;
    }
    else if( *ptr == '\r' )
    {
        ++ptr;
        if( !EmptyQ() && (*ptr == '\n') ) { ++ptr; }
    }
    else
    {
        failedQ = true;
    }
    
    return *this;
}
