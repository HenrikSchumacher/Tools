public:

template<
    typename Fmt = Format::Matrix::Tall,
    IntQ Int_0, IntQ Int_1, ArrayFun<Int_0,Int_1> A, CharConv<Result_T<A>> C
>
InString & TakeMatrix( A && a, C && from_chars, Int_0 d_0, Int_1 d_1 )
{
    return TakeArray(
        std::forward<A>(a), std::forward<C>(from_chars),
        d_0, Fmt::prefix_0, Fmt::infix_0, Fmt::suffix_0,
        d_1, Fmt::prefix_1, Fmt::infix_1, Fmt::suffix_1
    );
}

template<typename Fmt = Format::Matrix::Tall, IntQ Int_0, IntQ Int_1, ArrayFun<Int_0,Int_1> A>
InString & TakeMatrix( A && a, Int_0 d_0, Int_1 d_1  )
{
    return TakeMatrix<Fmt>(std::forward<A>(a), FromChars<Result_T<A>>(), d_0, d_1 );
}

template<typename Fmt = Format::Matrix::Tall, typename T, IntQ Int_0, IntQ Int_1, CharConv<T> C>
InString & TakeMatrix( cptr<T> a, C && from_chars, Int_0 d_0, Int_1 d_1 )
{
    return TakeMatrix<Fmt>(
        [a,d_1]( const Int_0 i_0, const Int_1 i_1 ) -> T { return a[i_0 * d_1 + i_1]; },
        std::forward<C>(from_chars), d_0, d_1
    );
}

template<typename Fmt = Format::Matrix::Tall, typename T, IntQ Int_0, IntQ Int_1>
InString & TakeMatrix( cptr<T> a, Int_0 d_0, Int_1 d_1 )
{
    return TakeMatrix<Fmt>(a, FromChars<T>(), d_0, d_1);
}
