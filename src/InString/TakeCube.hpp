public:

template<
    typename Fmt = Format::Cube::Medium,
    IntQ Int_0, IntQ Int_1, IntQ Int_2, ArrayFun<Int_0,Int_1,Int_2> A, CharConv<Result_T<A>> C
>
InString & TakeCube( A && a, C && from_chars, Int_0 d_0, Int_1 d_1, Int_2 d_2 )
{
    return TakeArray(
        std::forward<A>(a), std::forward<C>(from_chars),
        d_0, Fmt::prefix_0, Fmt::infix_0, Fmt::suffix_0,
        d_1, Fmt::prefix_1, Fmt::infix_1, Fmt::suffix_1,
        d_2, Fmt::prefix_2, Fmt::infix_2, Fmt::suffix_2
    );
}

template<
    typename Fmt = Format::Cube::Medium,
    IntQ Int_0, IntQ Int_1, IntQ Int_2, ArrayFun<Int_0,Int_1,Int_2> A
>
InString & TakeCube( A && a, Int_0 d_0, Int_1 d_1, Int_2 d_2 )
{
    return TakeCube<Fmt>(std::forward<A>(a), FromChars<Result_T<A>>(), d_0, d_1, d_2);
}

template<
    typename Fmt = Format::Cube::Medium,
    typename T, IntQ Int_0, IntQ Int_1, IntQ Int_2, CharConv<T> C
>
InString & TakeCube( cptr<T> a, C && from_chars, Int_0 d_0, Int_1 d_1, Int_2 d_2 )
{
    return TakeCubeFun<Fmt>(
        [a,d_1,d_2]( const Int_0 i_0, const Int_1 i_1, const Int_2 i_2 )
        {
            return a[(d_1 * i_0 + i_1) * d_2 + i_2];
        },
        std::forward<C>(from_chars), d_0, d_1, d_2
    );
}

template<
    typename Fmt = Format::Cube::Medium,
    typename T, IntQ Int_0, IntQ Int_1, IntQ Int_2
>
InString & TakeCube( cptr<T> a, Int_0 d_0, Int_1 d_1, Int_2 d_2 )
{
    return TakeCube<Fmt>(a, FromChars<T>(), d_0, d_1, d_2);
}
