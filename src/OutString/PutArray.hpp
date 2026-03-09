public:


template<bool allocatedQ = false, NonPointerQ EntryFun_T, NonPointerQ ToChars_T, typename ...Args>
OutString & PutArray(
    EntryFun_T && a, ToChars_T && to_chars, bool check_sizeQ, Args&&... args
)
{
    static_assert(ToChars_T::implementedQ,"");
    
    if constexpr ( allocatedQ )
    {
    }
    else
    {
        if ( check_sizeQ )
        {
            const Size_T full_size = ArrayCharCount(
                std::forward<ToChars_T>(to_chars), std::forward<Args>(args)...
            );
            RequireFreeSpace(full_size);
        }
    }
    
    putArray(
        std::forward<EntryFun_T>(a),
        std::forward<ToChars_T>(to_chars),
        std::forward<Args>(args)...
     );

    return *this;
}

template<NonPointerQ EntryFun_T, typename ...Args>
OutString & PutArray( EntryFun_T && a, bool check_sizeQ, Args&&... args )
{
    using T = typename std::remove_reference<typename function_traits<EntryFun_T>::return_type>::type;
    
    return PutArray(
        std::forward<EntryFun_T>(a),
        ToChars<T>(),
        check_sizeQ,
        std::forward<Args>(args)...
    );
}

private:

template<
    NonPointerQ EntryFun_T, NonIntQ ToChars_T,
    IntQ Int, NonIntQ Prefix_T, NonIntQ Infix_T, NonIntQ Suffix_T,
    typename ...Args
>
constexpr void putArray(
    EntryFun_T && a, ToChars_T && to_chars,
    Int n, Prefix_T && prefix, Infix_T  && infix, Suffix_T && suffix,
    Args&&... args
)
{
    PutChars<false>(prefix);
    if( n > Int(0) )
    {
        const Int i = 0;
        putArray( std::bind_front(a,i), std::forward<ToChars_T>(to_chars), std::forward<Args>(args)... );
    }
    for( Int i = 1; i < n; ++i )
    {
        PutChars<false>(infix);
        putArray( std::bind_front(a,i), std::forward<ToChars_T>(to_chars), std::forward<Args>(args)... );
    }
    PutChars<false>(suffix);
}


template<
    NonPointerQ EntryFun_T, NonIntQ ToChars_T,
    IntQ Int, NonIntQ Prefix_T, NonIntQ Infix_T, NonIntQ Suffix_T
>
constexpr void putArray(
    EntryFun_T && a, ToChars_T && to_chars,
    Int n, Prefix_T && prefix, Infix_T  && infix, Suffix_T && suffix
)
{
    PutChars<false>(prefix);
    if( n > Int(0) )
    {
        const Int i = 0;
        Put<false>(a(i),std::forward<ToChars_T>(to_chars));
    }
    for( Int i = 1; i < n; ++i )
    {
        PutWithPrefix<false>(infix,a(i),std::forward<ToChars_T>(to_chars));
    }
    PutChars<false>(suffix);
}

//template<
//    typename EntryFun_T, typename ToChars_T
//>
//void putArray(
//    EntryFun_T && a, ToChars_T && to_chars
//)
//{
//    Put<false>(a(),std::forward<ToChars_T>(to_chars));
//}



public:


template<typename T, IntQ Int, typename ...Args>
static constexpr Size_T ArrayCharCount(
    Int n, Args&&... args
)
{
    return arrayCharCount( ToChars<T>(), n, std::forward<Args>(args)... );
}

template<NonPointerQ ToChars_T, typename ...Args>
static constexpr Size_T ArrayCharCount(
    ToChars_T && to_chars, Args&&... args
)
{
    return arrayCharCount( std::forward<ToChars_T>(to_chars), std::forward<Args>(args)... );
}

private:

template<
    NonPointerQ ToChars_T,
    IntQ Int, NonIntQ Prefix_T, NonIntQ Infix_T, NonIntQ Suffix_T,
    typename ...Args
>
static constexpr Size_T arrayCharCount(
    ToChars_T && to_chars,
    Int n, Prefix_T && prefix, Infix_T  && infix, Suffix_T && suffix,
    Args&&... args
)
{
    Size_T size = arrayCharCount( std::forward<ToChars_T>(to_chars), std::forward<Args>(args)...) + CharCount(infix);
    
    return ToSize_T(n) * size + CharCount(prefix) + CharCount(suffix);
}

template<NonPointerQ ToChars_T>
static constexpr Size_T arrayCharCount(
    ToChars_T && to_chars
)
{
    return to_chars.char_count;
}

