#pragma once

namespace Tools
{
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename F
//        typename S = decltype( F()(Size_T(0)) )
    >
    auto MinMaxBy(
        F && f, const Size_T n = N, const Size_T thread_count = 1
    ) -> std::pair<decltype(f(Size_T(0))),decltype(f(Size_T(0)))>
    {
        using S = decltype(f(Size_T(0)));
        
        using T = std::pair<S,S>;
        
        T init { std::numeric_limits<S>::max(), std::numeric_limits<S>::lowest() };
        
        return DoReduce<N,parQ>(
            [&f]( const Size_T i ) -> T
            {
                const S f_i = f(i);
                
                return T(f_i,f_i);
            },
            []( cref<T> value, mref<T> result )
            {
                result.first  = Min( value.first,  result.first  );
                result.second = Max( value.second, result.second );
            },
            init, n, thread_count
        );
    }
    
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE std::pair<S,S>
    minmax_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1)
    {
        return MinMaxBy<N,parQ>( [z]( const Size_T i ){ return z[i]; }, n, thread_count );
    }
    
    
    
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename F >
    auto MinBy(
        F && f, const Size_T n = N, const Size_T thread_count = 1
    ) -> decltype(f(Size_T(0)))
    {
        using T = decltype(f(Size_T(0)));
        
        return DoReduce<N,parQ>(
            [&f]( const Size_T i ) -> T
            {
                return f(i);
            },
            []( const T & value, T & result )
            {
                result = Min( result, value );
            },
            std::numeric_limits<T>::max(), n, thread_count
        );
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename F >
    auto MinimumBy( 
        F && f, const Size_T n = N, const Size_T thread_count = 1
    ) -> std::pair<Size_T,decltype(f(Size_T(0)))>
    {
        using S = decltype( f(Size_T(0)) );
        
        using T = std::pair<Size_T,S>;
        
        T init { Size_T(0), std::numeric_limits<S>::max() };
        
        return DoReduce<N,parQ>(
            [&f]( const Size_T i ) -> T
            {
                const S f_i = f(i);
                
                return std::tie(i,f_i);
            },
            []( cref<T> value, mref<T> result )
            {
                if( value.second < result.second )
                {
                    result = value;
                }
            },
            init, n, thread_count
        );
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE S 
    min_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1 )
    {
        return MinBy<N,parQ>(
            [z]( const Size_T i ){ return z[i]; }, n, thread_count
        );
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE S 
    min_pos_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1 )
    {
        return MinimumBy<N,parQ>(
            [z]( const Size_T i ){ return z[i]; }, n, thread_count
        ).first;
    }
    
    
    

    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename F >
    auto MaxBy(
        F && f, const Size_T n = N, const Size_T thread_count = 1
    ) -> decltype(f(Size_T(0)))
    {
        using S = decltype(f(Size_T(0)));
        
        return DoReduce<N,parQ>(
            [&f]( const Size_T i ) -> S
            {
                return f(i);
            },
            []( const S & value, S & result )
            {
                result = Max( result, value );
            },
            std::numeric_limits<S>::lowest(), n, thread_count
        );
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename F >
    auto MaximumBy(
        F && f, const Size_T n = N, const Size_T thread_count = 1
    ) -> std::pair<Size_T,decltype(f(Size_T(0)))>
    {
        using S = decltype(f(Size_T()));
        
        using T = std::pair<Size_T,S>;
        
        T init { Size_T(0), std::numeric_limits<S>::lowest() };
        
        return DoReduce<N,parQ>(
            [&f]( const Size_T i ) -> T
            {
                const S f_i = f(i);
                
                return std::pair(i,f_i);
            },
            []( cref<T> value, mref<T> result )
            {
                if( value.second > result.second )
                {
                    result = value;
                }
            },
            init, n, thread_count
        );
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE S 
    max_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1 )
    {
        return MaxBy<N,parQ>(
            [z]( const Size_T i ){ return z[i]; }, n, thread_count
        );
    }
    
    template< Size_T N = VarSize, Parallel_T parQ = Sequential, typename S >
    [[nodiscard]] TOOLS_FORCE_INLINE S 
    max_pos_buffer( cptr<S> z, const Size_T n = N, const Size_T thread_count = 1 )
    {
        return MaximumBy<N,parQ>(
            [z]( const Size_T i ){ return z[i]; }, n, thread_count
        ).first;
    }
    
    
} // namespace Tools
