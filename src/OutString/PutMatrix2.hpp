public:

template<
    typename T, IntQ Int,
    const Size_T n_prefix_0,
    const Size_T n_infix_0,
    const Size_T n_suffix_0,
    const Size_T n_prefix_1,
    const Size_T n_infix_1,
    const Size_T n_suffix_1,
    typename ToChars_T = ToChars<T>
>
OutString & PutMatrix( cptr<T> a, Int m, Int n,
    const char(&prefix_0)[n_prefix_0],
    const char(&infix_0 )[n_infix_0 ],
    const char(&suffix_0)[n_suffix_0],
    const char(&prefix_1)[n_prefix_1],
    const char(&infix_1 )[n_infix_1 ],
    const char(&suffix_1)[n_suffix_1],
    bool check_sizeQ = true,
    ToChars_T && to_chars = ToChars_T()
)
{
    return PutMatrixFunction(
        [a,n]( const Int i, const Int j ) { return a[n * i + j]; },
        m, n, prefix_0, infix_0, suffix_0, prefix_1, infix_1, suffix_1,
        check_sizeQ, std::forward<ToChars_T>(to_chars)
    );
}

template<
    typename MatrixFun_T, IntQ Int,
    const Size_T n_prefix_0,
    const Size_T n_infix_0,
    const Size_T n_suffix_0,
    const Size_T n_prefix_1,
    const Size_T n_infix_1,
    const Size_T n_suffix_1,
    typename ToChars_T = ToChars<typename std::remove_reference<typename function_traits<MatrixFun_T>::return_type>::type>
>
OutString & PutMatrixFunction( MatrixFun_T && a, Int d_0, Int d_1,
    const char(&prefix_0)[n_prefix_0],
    const char(&infix_0 )[n_infix_0 ],
    const char(&suffix_0)[n_suffix_0],
    const char(&prefix_1)[n_prefix_1],
    const char(&infix_1 )[n_infix_1 ],
    const char(&suffix_1)[n_suffix_1],
    bool check_sizeQ = true,
    ToChars_T && to_chars = ToChars_T()
)
{
    static_assert(ToChars_T::implementedQ,"");
    
    if ( check_sizeQ )
    {
        Size_T entry_size = to_chars.char_count + n_infix_1;
        Size_T row_size   = entry_size * d_1 + n_prefix_1 + n_suffix_1 + n_infix_0;
        Size_T full_size  = row_size * d_0 + n_prefix_0 + n_suffix_0;
        
        this->RequireFreeSpace(full_size);
    }
    
    PutChars<false>(prefix_0);
    if( d_0 > Int(0) )
    {
        const Int i = 0;
        PutChars<false>(prefix_1);
        if( d_1 > Int(0) )
        {
            const Int j = 0;
//            print_data(i,j);
            Put<false>(a(i,j),std::forward<ToChars_T>(to_chars));
        }
        for( Int j = 1; j < d_1; ++j )
        {
//            print_data(i,j);
            PutWithPrefix<false>(infix_1,a(i,j),std::forward<ToChars_T>(to_chars));
        }
        PutChars<false>(suffix_1);
    }
    for( Int i = 1; i < d_0; ++i )
    {
        PutChars<false>(infix_0);
        PutChars<false>(prefix_1);
        if( d_1 > Int(0) )
        {
            const Int j = 0;
//            print_data(i,j);
            Put<false>(a(i,j),std::forward<ToChars_T>(to_chars));
        }
        for( Int j = 1; j < d_1; ++j )
        {
//            print_data(i,j);
            PutWithPrefix<false>(infix_1,a(i,j),std::forward<ToChars_T>(to_chars));
        }
        PutChars<false>(suffix_1);
    }
    PutChars<false>(suffix_0);
    
    return *this;
}






template<
    typename T, IntQ Int,
    const Size_T n_prefix_0,
    const Size_T n_infix_0,
    const Size_T n_suffix_0,
    const Size_T n_prefix_1,
    const Size_T n_infix_1,
    const Size_T n_suffix_1,
    typename ToChars_T = ToChars<T>
>
OutString & PutMatrix2( cptr<T> a, Int m, Int n,
    const char(&prefix_0)[n_prefix_0],
    const char(&infix_0 )[n_infix_0 ],
    const char(&suffix_0)[n_suffix_0],
    const char(&prefix_1)[n_prefix_1],
    const char(&infix_1 )[n_infix_1 ],
    const char(&suffix_1)[n_suffix_1],
    bool check_sizeQ = true,
    ToChars_T && to_chars = ToChars_T()
)
{
    return PutMatrixFunction2(
        [a,n]( const Int i, const Int j ) { return a[n * i + j]; },
        m, n, prefix_0, infix_0, suffix_0, prefix_1, infix_1, suffix_1,
        check_sizeQ, std::forward<ToChars_T>(to_chars)
    );
}

template<
    typename MatrixFun_T, IntQ Int,
    const Size_T n_prefix_0,
    const Size_T n_infix_0,
    const Size_T n_suffix_0,
    const Size_T n_prefix_1,
    const Size_T n_infix_1,
    const Size_T n_suffix_1,
    typename ToChars_T = ToChars<typename std::remove_reference<typename function_traits<MatrixFun_T>::return_type>::type>
>
OutString & PutMatrixFunction2( MatrixFun_T && a, Int d_0, Int d_1,
    const char(&prefix_0)[n_prefix_0],
    const char(&infix_0 )[n_infix_0 ],
    const char(&suffix_0)[n_suffix_0],
    const char(&prefix_1)[n_prefix_1],
    const char(&infix_1 )[n_infix_1 ],
    const char(&suffix_1)[n_suffix_1],
    bool check_sizeQ = true,
    ToChars_T && to_chars = ToChars_T()
)
{
    static_assert(ToChars_T::implementedQ,"");
    
    if ( check_sizeQ )
    {
        Size_T entry_size = to_chars.char_count + n_infix_1;
        Size_T row_size   = entry_size * d_1 + n_prefix_1 + n_suffix_1 + n_infix_0;
        Size_T full_size  = row_size * d_0 + n_prefix_0 + n_suffix_0;
        
        this->RequireFreeSpace(full_size);
    }
    
    PutChars<false>(prefix_0);
    if( d_0 > Int(0) )
    {
        const Int i = 0;
        PutVectorFunction(
            std::bind_front(a,i),
            d_1, prefix_1, infix_1, suffix_1,
            false, std::forward<ToChars_T>(to_chars)
        );
    }
    for( Int i = 1; i < d_0; ++i )
    {
        PutChars<false>(infix_0);
        PutVectorFunction(
            std::bind_front(a,i),
            d_1, prefix_1, infix_1, suffix_1,
            false, std::forward<ToChars_T>(to_chars)
        );
    }
    PutChars<false>(suffix_0);
    
    return *this;
}
