public:

template<
    typename Fmt = Format::Vector::Default,
    IntQ Int_0, ArrayFun<Int_0> A, CharConv<Result_T<A>> C
>
InString & TakeVector( A && a, C && from_chars, Int_0 d_0 )
{
    
    return TakeArray(
        std::forward<A>(a), std::forward<C>(from_chars),
        d_0, Fmt::prefix_0, Fmt::infix_0, Fmt::suffix_0
    );
}

template<typename Fmt = Format::Vector::Default, IntQ Int_0, ArrayFun<Int_0> A>
InString & TakeVector( A && a, Int_0 d_0 )
{
    return TakeArray(
        std::forward<A>(a), FromChars<Result_T<A>>(),
        d_0, Fmt::prefix_0, Fmt::infix_0, Fmt::suffix_0
    );
}

template<typename Fmt = Format::Vector::Default, typename T, IntQ Int_0, CharConv<T> C>
InString & TakeVector( cptr<T> a, C && from_chars, Int_0 d_0 )
{
    return TakeVector<Fmt>( [a]( const Int_0 i_0 ) { return a[i_0]; }, std::forward<C>(from_chars), d_0 );
}

template<typename Fmt = Format::Vector::Default, typename T, IntQ Int_0>
InString & TakeVector( cptr<T> a, Int_0 d_0 )
{
    return TakeVector<Fmt>( [a]( const Int_0 i_0 ) { return a[i_0]; }, FromChars<T>(), d_0 );
}
