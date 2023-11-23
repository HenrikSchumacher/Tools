#pragma once

namespace Tools
{
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_buffers(
        cref<a_T> a, cptr<x_T> x, 
        cref<b_T> b, mptr<y_T> y,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        // This routine computes y[i] = a * x[i] + b * y[i].
        //
        // Depending on the values of a_flag and b_flag, it takes several short cuts:
        // If a_flag == Flag::Zero,  then it assumes a = 0.
        // If a_flag == Flag::Plus,  then it assumes a = 1.
        // If a_flag == Flag::Minus, then it assumes a = -1.
        // If a_flag == Flag::Generic, then it assumes generic values for a.
        
        // If b_flag == Flag::Zero,  then it assumes b = 0.
        // If b_flag == Flag::Plus,  then it assumes b = 1.
        // If b_flag == Flag::Minus, then it assumes b = -1.
        // If b_flag == Flag::Generic, then it assumes generic values for b.
        
        using namespace Scalar;
        
        check_sequential<parQ>( "combine_buffers", thread_count );
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "combine_buffers: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "combine_buffers: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        static_assert( ComplexQ<y_T> || (RealQ<a_T> && RealQ<x_T> && RealQ<b_T>),
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `a` and `b` to the right precision because this is better done once before any loop that calls `combine_buffers`. Hence we prefer a compile error here.
        
        static_assert(
            Prec<a_T> == Prec<y_T>,
            "Precisions of first and fourth argument do not coincide."
        );
        
        static_assert(
            Prec<b_T> == Prec<y_T>,
            "Precisions of third and fourth argument do not coincide."
        );
        
        if constexpr ( (a_flag == Flag::Zero) && (b_flag == Flag::Zero) )
        {
            zerofy_buffer<N,parQ>(y,n,thread_count);
        }
        else if constexpr (
            (b_flag == Flag::Zero) && (a_flag == Flag::Plus) && ( opx == Op::Id )
        )
        {
            copy_buffer<N,parQ>(x,y,n,thread_count);
        }
        else if constexpr (
            (a_flag == Flag::Zero) && (b_flag == Flag::Plus) && ( opx == Op::Id )
        )
        {
            // Leave y as it is.
        }
        else
        {
            if constexpr ( (N > VarSize) && VectorizableQ<y_T> && ( opx == Op::Id ) && (opy == Op::Id ) )
            {
                vec_T<N,y_T> x_vec;
                vec_T<N,y_T> y_vec;
                
                // All this copying looks insane, but the clang compiler happily optimizes it away...
                
                copy_buffer<N>( x, reinterpret_cast<y_T *>(&x_vec) );
                copy_buffer<N>( y, reinterpret_cast<y_T *>(&y_vec) );

                y_vec = static_cast<y_T>(a) * x_vec + static_cast<y_T>(b) * y_vec;

                copy_buffer<N>( reinterpret_cast<y_T *>(&y_vec), y );
                
            }
            else
            {
                Do<N,parQ,Static>(
                    [=]( const Size_T i )
                    {
                        combine_scalars<a_flag,b_flag,opx,opy>( a, x[i], b, y[i] );
                    },
                    n, thread_count
                );
            }
        }
    }
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Size_T N = VarSize, Parallel_T parQ = Sequential,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T, typename z_T
    >
    force_inline void combine_buffers(
        cref<a_T> a, cptr<x_T> x, cref< b_T> b, cptr<y_T> y, mptr<z_T> z,
        const Size_T n = N,
        const Size_T thread_count = 1
    )
    {
        // This routine computes z[i] = a * x[i] + b * y[i].
        // Most importantly, this casts the result to the desired type.
        //
        // Depending on the values of a_flag and b_flag, it takes several short cuts:
        // If a_flag == Flag::Zero,  then it assumes a = 0.
        // If a_flag == Flag::Plus,  then it assumes a = 1.
        // If a_flag == Flag::Minus, then it assumes a = -1.
        // If a_flag == Flag::Generic, then it assumes generic values for a.
        
        // If b_flag == Flag::Zero,  then it assumes b = 0.
        // If b_flag == Flag::Plus,  then it assumes b = 1.
        // If b_flag == Flag::Minus, then it assumes b = -1.
        // If b_flag == Flag::Generic, then it assumes generic values for b.
        
        
        using namespace Scalar;
        
        check_sequential<parQ>( "combine_buffers", thread_count );
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "combine_buffers: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "combine_buffers: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        static_assert( ComplexQ<z_T> || (RealQ<a_T> && RealQ<x_T> && RealQ<b_T> && RealQ<y_T> ),
            "Last argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `a` and `b` to the right precision because this is better done once before any loop that calls `combine_buffers`. Hence we prefer a compile error here.
        
        static_assert(
            Prec<a_T> == Prec<z_T>,
            "Precisions of first and last argument do not coincide."
        );
        
        static_assert(
            Prec<b_T> == Prec<z_T>,
            "Precisions of third and last argument do not coincide."
        );
        
        if constexpr ( (a_flag == Flag::Zero) && (b_flag == Flag::Zero) )
        {
            // In order to make it safe that x == nullptr and y == nullptr, we nead a special treatment here.
            
            zerofy_buffer<N,parQ>(z,n,thread_count);
        }
        else if constexpr ( b_flag == Flag::Zero )
        {
            // In order to make it safe that y == nullptr, we nead a special treatment here.
            
            if constexpr ( (a_flag == Flag::Plus) && (opx == Op::Id) )
            {
                // We do a copy if we can.
                copy_buffer<N,parQ>(x,z,n,thread_count);
            }
            if constexpr ( (N != VarSize) && VectorizableQ<z_T> && (opx == Op::Id) )
            {
                // We make use of vectorization if we can.
                
                vec_T<N,z_T> x_vec;
                vec_T<N,z_T> z_vec;
                
                // All this copying looks insane, but the clang compiler happily optimizes it away...

                copy_buffer<N>( x, reinterpret_cast<z_T *>(&x_vec) );

                z_vec = static_cast<z_T>(a) * x_vec;

                copy_buffer<N>( reinterpret_cast<z_T *>(&z_vec), z );
            }
            else
            {
                // Fallback / most general routine.
                
                Do<N,parQ,Static>(
                    [=]( const Size_T i )
                    {
                        constexpr auto ox = COND( opx == Op::Conj,
                                []( const x_T & X ){ return scalar_cast<z_T>( Conj<x_T>(X) ); },
                                []( const x_T & X ){ return scalar_cast<z_T>( X ); }
                        );
                        
                        z[i] = a * ox ( x[i] );
                    },
                    n, thread_count
                );
            }
            
        }
        else if constexpr (a_flag == Flag::Zero)
        {
            // In order to make it safe that x == nullptr, we nead a special treatment here.
            
            if constexpr ( (b_flag == Flag::Plus) && (opy == Op::Id) )
            {
                // We do a copy if we can.
                
                copy_buffer<N,parQ>(y,z,n,thread_count);
            }
            if constexpr ( (N != VarSize) && VectorizableQ<z_T> && (opy == Op::Id) )
            {
                // We make use of vectorization if we can.
                
                vec_T<N,z_T> y_vec;
                vec_T<N,z_T> z_vec;
                
                // All this copying looks insane, but the clang compiler happily optimizes it away...

                copy_buffer<N>( x, reinterpret_cast<z_T *>(&y_vec) );

                z_vec = static_cast<z_T>(a) * y_vec;

                copy_buffer<N>( reinterpret_cast<z_T *>(&z_vec), z );
            }
            else
            {
                Do<N,parQ,Static>(
                    [=]( const Size_T i )
                    {
                        constexpr auto oy = COND( opy == Op::Conj,
                                []( const y_T & Y ){ return scalar_cast<y_T>( Conj<y_T>(Y) ); },
                                []( const y_T & Y ){ return scalar_cast<y_T>( Y ); }
                        );
                        
                        z[i] = b * oy( y[i] );
                    },
                    n, thread_count
                );
            }
        }
        else
        {
            if constexpr ( (N != VarSize) && VectorizableQ<z_T> && ( opx == Op::Id ) && (opy == Op::Id ) )
            {
                vec_T<N,z_T> x_vec;
                vec_T<N,z_T> y_vec;
                vec_T<N,z_T> z_vec;
                
                // All this copying looks insane, but the clang compiler happily optimizes it away...

                copy_buffer<N>( x, reinterpret_cast<z_T *>(&x_vec) );
                copy_buffer<N>( y, reinterpret_cast<z_T *>(&y_vec) );

                z_vec = static_cast<z_T>(a) * x_vec + static_cast<z_T>(b) * y_vec;

                copy_buffer<N>( reinterpret_cast<z_T *>(&z_vec), z );
            }
            else
            {
                Do<N,parQ,Static>(
                    [=]( const Size_T i )
                    {
                        combine_scalars<a_flag,b_flag,opx,opy>( a, x[i], b, y[i], z[i] );
                    },
                    n, thread_count
                );
            }
        }
    }

} // namespace Tools
