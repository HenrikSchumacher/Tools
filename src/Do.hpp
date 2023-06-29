#pragma once

//Do(fun,n) equals Do<VarSize,Sequential>fun(fun,n)
//Do<VarSize,Parallel>(fun,n,thread_count) equals Do<Variable,Parallel,Static>(fun,n,thread_count)
//Do<VarSize,Parallel,Dynamic>(fun,n,thread_count)
//Do<N>(fun)


namespace Tools
{

    static constexpr Size_T VarSize = 0;


    enum struct Parallel_T : bool
    {
        True  = true,
        False = false
    };
    
    static constexpr Parallel_T Parallel   = Parallel_T::True;

    static constexpr Parallel_T Sequential = Parallel_T::False;
    
    enum struct Dynamic_T : bool
    {
        True  = true,
        False = false
    };
    
    static constexpr Dynamic_T Dynamic = Dynamic_T::True;

    static constexpr Dynamic_T Static  = Dynamic_T::False;
    
    
    
    template<Dynamic_T dynQ>
    void constexpr not_dynamic()
    {
        static_assert(dynQ == Static, "This method ignores dynamic scheduling; it is always static.");
    }
    
#ifdef TOOLS_DEBUG
    template<Parallel_T parQ, typename Int >
    void check_sequential( std::string tag, Int thread_count )
    {
        if constexpr ( parQ == Sequential )
        {
            if ( thread_count > Scalar::One<Int> )
            {
                wprint(std::string("check_sequential: method ") + tag + " is marked as sequential, but more than 1 thread is requested.");
            }
        }
    }
#else
    
    template<Parallel_T parQ, typename Int >
    void check_sequential( const char * tag, Int thread_count )
    {}
#endif
    
    
    
    // Executes the function `fun` of the form `[]( const Int i ) -> void {...}` on the range [begin,end[, parallelized over `thread_count` threads.
    // This allows also loops with constant trip counts `N`; if `N > 0`, then the loop will be unrolled and processed sequentially.
    // Parallelization will be turned of also if `parQ` is set to `Sequential`.
    template<
        Size_T N = VarSize, Parallel_T parQ = Sequential, Dynamic_T dynQ = Static,
        typename F, typename Int, typename Int_
    >
    force_inline void Do( F && fun, const Int n = N, const Int_ thread_count_ = 1 )
    {
        ASSERT_INT(Int);
        ASSERT_INT(Int_);
        
        
        const Int thread_count = static_cast<Int>(thread_count_);
        
        check_sequential<parQ>( "Do", thread_count );
        
        constexpr Int zero = Scalar::Zero<Int>;
        constexpr Int one  = Scalar::One <Int>;
        
#ifdef TOOLS_DEBUG
        if constexpr ( N == VarSize )
        {
            if( n == zero )
            {
                wprint("Do: Input length n is 0 although variable size inputs are specified.");
                return;
            }
        }
        else
        {
            if( N != int_cast<Size_T>(n) )
            {
                wprint("Do: Input n does not coincide with template parameter N.");
            }
            
            if constexpr ( parQ == Parallel_T::True )
            {
                if( thread_count_ <= one )
                {
                    wprint("Do: Parallelization activated, but thread_count <= 1.");
                }
            }
            else
            {
                if( thread_count_ > zero )
                {
                    wprint("Do: Parallelization deactivated, but thread_count > 0.");
                }
            }
        }
#endif
        
        if constexpr ( N == VarSize )
        {
            if constexpr (parQ == Parallel_T::True)
            {
                if( thread_count <= one )
                {
                    for( Int i = zero; i < n; ++i ) { fun(i); }
                }
                else
                {
                    if constexpr ( dynQ == Dynamic_T::True )
                    {
                        ParallelDo_Dynamic( std::forward<F>(fun), zero, n, one, thread_count );
                    }
                    else
                    {
                        ParallelDo( std::forward<F>(fun), zero, n, thread_count );
                    }
                }
            }
            else
            {
                for( Int i = zero; i < n; ++i ) { fun(i); }
            }
        }
        else
        {
            for( Int i = zero; i < N; ++i ) { fun(i); }
        }
    }
}
