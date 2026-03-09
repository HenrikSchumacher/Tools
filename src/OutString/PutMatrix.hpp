public:

template<
    MatrixFormat format = MatrixFormat::Tall,
    NonPointerQ EntryFun_T, IntQ Int_0, IntQ Int_1,
    NonIntQ ToChars_T = ToChars<Result_T<EntryFun_T>>
>
OutString & PutMatrixFun(
    EntryFun_T && a, Int_0 d_0, Int_1 d_1, ToChars_T && to_chars = ToChars_T()
)
{
    if( format == MatrixFormat::Tall )
    {
        const char prefix_0 [3] = "{\n";
        const char infix_0  [3] = ",\n";
        const char suffix_0 [3] = "\n}";
        const char prefix_1 [4] = " { ";
        const char infix_1  [3] = ", ";
        const char suffix_1 [3] = " }";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1
        );
    }
    else if constexpr ( format == MatrixFormat::Wide )
    {
        const char prefix_0 [3] = "{ ";
        const char infix_0  [3] = ", ";
        const char suffix_0 [3] = " }";
        const char prefix_1 [4] = "{ ";
        const char infix_1  [3] = ", ";
        const char suffix_1 [3] = " }";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1
        );
    }
    else if constexpr ( format == MatrixFormat::TSV )
    {
        const char prefix_0 [1] = "";
        const char infix_0  [2] = "\n";
        const char suffix_0 [1] = "";
        const char prefix_1 [1] = "";
        const char infix_1  [2] = "\t";
        const char suffix_1 [1] = "";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1
        );
    }
    else if constexpr ( format == MatrixFormat::CSV )
    {
        const char prefix_0 [1] = "";
        const char infix_0  [2] = "\n";
        const char suffix_0 [1] = "";
        const char prefix_1 [1] = "";
        const char infix_1  [2] = ",";
        const char suffix_1 [1] = "";
        
        return PutArray(
            std::forward<EntryFun_T>(a), std::forward<ToChars_T>(to_chars), true,
            d_0, prefix_0, infix_0, suffix_0,
            d_1, prefix_1, infix_1, suffix_1
        );
    }
}

template<
    MatrixFormat format = MatrixFormat::Tall,
    typename T, IntQ Int_0, IntQ Int_1, NonIntQ ToChars_T = ToChars<T>
>
OutString & PutMatrix( cptr<T> a, Int_0 d_0, Int_1 d_1, ToChars_T && to_chars = ToChars_T() )
{
    return PutMatrixFun<format>(
        [a,d_1]( const Int_0 i_0, const Int_1 i_1 )
        {
            return a[i_0 * d_1 + i_1];
        },
        d_0, d_1, std::forward<ToChars_T>(to_chars)
    );
}
