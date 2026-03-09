public:

template<
    CubeFormat format = CubeFormat::Medium,
    NonPointerQ EntryFun_T, IntQ Int_0, IntQ Int_1, IntQ Int_2,
    NonIntQ ToChars_T = ToChars<Result_T<EntryFun_T>>
>
OutString & PutCubeFun(
    EntryFun_T && a, Int_0 d_0, Int_1 d_1, Int_2 d_2, ToChars_T && to_chars = ToChars_T()
)
{
    if constexpr ( format == CubeFormat::Tall )
    {
        char prefix_0 [3] = "{\n";
        char infix_0  [3] = ",\n";
        char suffix_0 [3] = "\n}";
        char prefix_1 [4] = " {\n";
        char infix_1  [3] = ",\n";
        char suffix_1 [4] = "\n }";
        char prefix_2 [5] = "  { ";
        char infix_2  [3] = ", ";
        char suffix_2 [3] = " }";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1,
            d_2, prefix_2, infix_2, suffix_2
        );
    }
    else if constexpr ( format == CubeFormat::Wide )
    {
        char prefix_0 [3] = "{ ";
        char infix_0  [3] = ", ";
        char suffix_0 [3] = " }";
        char prefix_1 [3] = "{ ";
        char infix_1  [3] = ", ";
        char suffix_1 [3] = " }";
        char prefix_2 [3] = "{ ";
        char infix_2  [3] = ", ";
        char suffix_2 [3] = " }";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1,
            d_2, prefix_2, infix_2, suffix_2
        );
    }
    else if constexpr ( format == CubeFormat::Medium )
    {
        char prefix_0 [3] = "{\n";
        char infix_0  [3] = ",\n";
        char suffix_0 [3] = "\n}";
        char prefix_1 [4] = " { ";
        char infix_1  [3] = ", ";
        char suffix_1 [3] = " }";
        char prefix_2 [3] = "{ ";
        char infix_2  [3] = ", ";
        char suffix_2 [3] = " }";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1,
            d_2, prefix_2, infix_2, suffix_2
        );
    }
}

template<
    CubeFormat format = CubeFormat::Medium,
    typename T, IntQ Int_0, IntQ Int_1, IntQ Int_2, NonIntQ ToChars_T = ToChars<T>
>
OutString & PutCube( cptr<T> a, Int_0 d_0, Int_1 d_1, Int_2 d_2, ToChars_T && to_chars = ToChars_T() )
{
    return PutCubeFun<CubeFormat>(
        [a,d_1,d_2]( const Int_0 i_0, const Int_1 i_1, const Int_2 i_2 )
        {
            return a[(d_1 * i_0 + i_1) * d_2 + i_2];
        },
        d_0, d_1, d_2, std::forward<ToChars_T>(to_chars)
    );
}
