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
