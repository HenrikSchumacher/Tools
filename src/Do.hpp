#pragma once

//Do(fun,n) equals Do<VarSize,Sequential>fun(fun,n)
//Do<VarSize,Parallel>(fun,n,thread_count) equals Do<Variable,Parallel,Static>(fun,n,thread_count)
//Do<VarSize,Parallel,Dynamic>(fun,n,thread_count)
//Do<N>(fun)


namespace Tools
{
    enum class Parallel_T : bool
    {
        True  = true,
        False = false
    };
    
    static constexpr Parallel_T Parallel   = Parallel_T::True;
    
    static constexpr Parallel_T Par        = Parallel_T::True;

    static constexpr Parallel_T Sequential = Parallel_T::False;
    
    static constexpr Parallel_T Seq        = Parallel_T::False;
    
    
    std::string ToString( const Parallel_T & parQ )
    {
        switch (parQ)
        {
            case Parallel:   return "Parallel";
            case Sequential: return "Sequential";
        }
    }
    
    
    template<IntQ Int, Parallel_T parQ, IntQ I>
    constexpr inline Int TruncateThreadCount( const I thread_count )
    {
        return (parQ == Parallel) ? Ramp_1(static_cast<Int>(thread_count)) : Int(1);
    }
    
    enum struct Dynamic_T : bool
    {
        True  = true,
        False = false
    };
    
    static constexpr Dynamic_T Dynamic = Dynamic_T::True;

    static constexpr Dynamic_T Static  = Dynamic_T::False;
    
    
    std::string ToString( const Dynamic_T & parQ )
    {
        switch (parQ)
        {
            case Dynamic: return "Dynamic";
            case Static:  return "Static";
        }
    }
    
    template<Dynamic_T dynQ>
    void constexpr not_dynamic()
    {
        static_assert(dynQ == Static, "This method ignores dynamic scheduling; it is always static.");
    }
    
    template<Parallel_T parQ, IntQ Int >
    void check_sequential( std::string tag, Int thread_count )
    {
#ifdef TOOLS_DEBUG
        if constexpr ( parQ == Sequential )
        {
            if ( thread_count > Int(1) )
            {
                wprint(std::string("check_sequential: method ") + tag + " is marked as sequential, but more than 1 thread is requested.");
            }
        }
#else
        (void)tag;
        (void)thread_count;
#endif
    }

    
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` on the range [begin,end[, parallelized over `thread_count` threads.
    // This allows also loops with constant trip counts `N`; if `N > 0`, then the loop will be unrolled and processed sequentially.
    // Parallelization will be turned of also if `parQ` is set to `Sequential`.
    template<
        Size_T N, Parallel_T parQ = Sequential, Dynamic_T dynQ = Static,
        typename F, IntQ Int = Size_T
    >
    TOOLS_FORCE_INLINE void Do(
        F && fun, const Int n = static_cast<Int>(N), const Int thread_count = 1
    )
    {
        check_sequential<parQ>( "Do", thread_count );
        
#ifdef TOOLS_DEBUG
        if constexpr ( N <= VarSize )
        {
//            if( n <= VarSize )
//            {
//                wprint("Do: Input length n = 0 although N = VarSize.");
//                return;
//            }
        }
        else
        {
            if( static_cast<Int>(N) != n )
            {
                wprint("Do: Input n does not coincide with template parameter N.");
            }
            
            if constexpr ( parQ == Parallel )
            {
                if( thread_count <= Int(1) )
                {
                    wprint("Do: Parallelization activated, but thread_count <= 1.");
                }
            }
            else
            {
                if( thread_count > Int(1) )
                {
                    wprint("Do: Parallelization deactivated, but thread_count > 1.");
                }
            }
        }
#endif // TOOLS_DEBUG
        
        if constexpr ( N <= VarSize )
        {
            if constexpr (parQ == Parallel)
            {
                if( thread_count <= Int(0) )
                {
                    for( Int i = 0; i < n; ++i )
                    {
                        fun(i);
                    }
                }
                else
                {
                    if constexpr ( dynQ == Dynamic )
                    {
                        ParallelDo_Dynamic( std::forward<F>(fun), Int(0), n, Int(1), thread_count );
                    }
                    else
                    {
                        ParallelDo( std::forward<F>(fun), Int(0), n, thread_count );
                    }
                }
            }
            else // if constexpr (parQ == Sequential)
            {
                (void)thread_count;
                for( Int i = 0; i < n; ++i ) { fun(i); }
            }
        }
        else
        {
            (void)thread_count;
            constexpr Int N_ = static_cast<Int>(N);
            for( Int i = 0; i < N_; ++i ) { fun(i); }
        }
    }
    
    
    template<Parallel_T parQ = Sequential, Dynamic_T dynQ = Static, typename F, IntQ Int>
    TOOLS_FORCE_INLINE void Do( F && fun, const Int n, const Int thread_count = 1 )
    {
        Do<VarSize,parQ,dynQ>( std::forward<F>(fun), n, thread_count);
    }
    
    template<Size_T N, Parallel_T parQ = Sequential,
        typename T, typename F, typename R, IntQ Int = Size_T
    >
    [[nodiscard]] TOOLS_FORCE_INLINE T DoReduce(
        F && fun, R && reducer, cref<T> init, const Int n = static_cast<Int>(N), const Int thread_count = 1
    )
    {
        // reducer must have prototype reducer( value, result );
        
        using S = decltype( fun( Int(0) ) );

        if constexpr ( N <= VarSize )
        {
            if constexpr ( parQ == Parallel )
            {
                return ParallelDoReduce(
                    std::forward<F>(fun),
                    // ParallelDoReduce expects reducer with
                    // prototype reducer( thread, value, result ).
                    [&reducer]( const Int thread, cref<S> value, mref<T> result )
                    {
                        (void)thread;
                        std::invoke( reducer, value, result );
                    },
                    init, Int(0), n, thread_count
                );
            }
            else
            {
                T result (init);
                for( Int i = 0; i < n; ++i ) { std::invoke( reducer, fun(i), result ); }
                return result;
            }
        }
        else
        {
            constexpr Int N_ = static_cast<Int>(N);
            T result (init);
            for( Int i = 0; i < N_; ++i ) { std::invoke( reducer, fun(i), result ); }
            return result;
        }
    }
    
    template<Parallel_T parQ = Sequential, typename T, typename F, typename R, IntQ Int>
    [[nodiscard]] TOOLS_FORCE_INLINE T DoReduce(
        F && fun, R && reducer, cref<T> init, const Int n, const Int thread_count = 1
    )
    {
        return Do<VarSize,parQ>(std::forward<F>(fun), std::forward<R>(reducer), init, n, thread_count);
    }
}
