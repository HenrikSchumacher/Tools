public:

template<
    NonPointerQ EntryFun_T, IntQ Int_0, NonIntQ ToChars_T = ToChars<Result_T<EntryFun_T>>
>
OutString & PutVectorFun(
    EntryFun_T && a, Int_0 d_0, ToChars_T && to_chars = ToChars_T()
)
{
    char prefix_0 [3] = "{ ";
    char infix_0  [3] = ", ";
    char suffix_0 [3] = " }";
    
    return PutArray(
        std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
        d_0, prefix_0, infix_0, suffix_0
    );
}

template<typename T, IntQ Int_0, NonIntQ ToChars_T = ToChars<T>>
OutString & PutVector( cptr<T> a, Int_0 d_0, ToChars_T && to_chars = ToChars_T() )
{
    return PutVectorFun(
        [a]( const Int_0 i_0 ) { return a[i_0]; },
        d_0, std::forward<ToChars_T>(to_chars)
    );
}
