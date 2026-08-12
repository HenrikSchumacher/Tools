public:

/*!@brief Put an array from the stream.
 *
 * This function takes separators between inputs very literal. It does _not_ ignore addition whitespace.
 *
 * @param a A write access function. A reference to the entry of the array at position `{i,j,...}` must be returned by `a(i,j,k,...)`. CAUTION: It is in the user's responsibility to make sure that the accessed array is allocated and large enough to store the inputs.
 *
 * @param from_chars A converter that converts chars to array entries; must have the same interface as the classes `FromChars`.
 *
 * @param n Leading dimension of array.
 *
 * @param args A sequence of groups of 4 arguments, each group being of the form `n`, `prefix`, `infix`, `suffix`. Here `n` must be of integral type and `prefix`, `infix`, `suffix` must satisfy the `Stringy` concept. (Typically, one wants to put string literals here.) So the separators in the stream must perfectly match the separators specified by the input sequence `args`. Otherwise, the read procedure will fail. The parameter `n` is supposed to belong to the first group if 4 arguments (it must be explicit for parsing variadic templates), so the first group in `args` must have only 3 parameters.
 */

template<NonPointerQ A, NonIntQ C, IntQ Int, typename ...Args>
InString & TakeArray( A && a, C && from_chars, Int n, Args&&... args)
{
    static_assert(C::implementedQ,"");
    
    takeArray(
        std::forward<A>(a),
        std::forward<C>(from_chars),
        n, std::forward<Args>(args)...
     );

    return *this;
}

/*!@brief Put an array from the stream.
 *
 * This function takes separators between inputs very literal. It does _not_ ignore addition whitespace.
 *
 * @param a A write access function. A reference to the entry of the array at position `{i,j,...}` must be returned by `a(i,j,k,...)`. CAUTION: It is in the user's responsibility to make sure that the accessed array is allocated and large enough to store the inputs.
 *
 * Precondition: An overload `FromChars<T>` must exist for the type `T`. Otherwise, a compile-time error is thrown.
 *
 * @param n Leading dimension of array.
 *
 * @param args A sequence of groups of 4 arguments, each group being of the form `n`, `prefix`, `infix`, `suffix`. Here `n` must be of integral type and `prefix`, `infix`, `suffix` must satisfy the `Stringy` concept. (Typically, one wants to put string literals here.) So the separators in the stream must perfectly match the separators specified by the input sequence `args`. Otherwise, the read procedure will fail. The parameter `n` is supposed to belong to the first group if 4 arguments (it must be explicit for parsing variadic templates), so the first group in `args` must have only 3 parameters.
 */


template<NonPointerQ A, IntQ Int, typename ...Args>
InString & TakeArray( A && a, Int n, Args&&... args )
{
    using T = typename std::remove_reference<typename function_traits<A>::return_type>::type;
    
    return TakeArray(
        std::forward<A>(a),
        FromChars<T>(),
        n, std::forward<Args>(args)...
    );
}


private:

template<
    NonPointerQ A, NonIntQ C, IntQ Int,
    Stringy Prefix_T, Stringy Infix_T, Stringy Suffix_T,
    typename ...Args
>
constexpr void takeArray(
    A && a, C && from_chars,
    Int n, Prefix_T && prefix, Infix_T  && infix, Suffix_T && suffix,
    Args&&... args
)
{
    SkipChars(prefix);
    if( n > Int(0) )
    {
        const Int i = 0;
        takeArray( std::bind_front(a,i), std::forward<C>(from_chars), std::forward<Args>(args)... );
    }
    for( Int i = 1; i < n; ++i )
    {
        SkipChars(infix);
        takeArray( std::bind_front(a,i), std::forward<C>(from_chars), std::forward<Args>(args)... );
    }
    SkipChars(suffix);
}


template<NonPointerQ A, NonIntQ C, IntQ Int, Stringy Prefix_T, Stringy Infix_T, Stringy Suffix_T>
constexpr void takeArray(
    A && a, C && from_chars,
    Int n, Prefix_T && prefix, Infix_T  && infix, Suffix_T && suffix
)
{
    SkipChars(prefix);
    if( n > Int(0) )
    {
        const Int i = 0;
        Take(a(i),std::forward<C>(from_chars));
    }
    for( Int i = 1; i < n; ++i )
    {
        SkipChars(infix);
        Take(a(i),std::forward<C>(from_chars));
    }
    SkipChars(suffix);
}
