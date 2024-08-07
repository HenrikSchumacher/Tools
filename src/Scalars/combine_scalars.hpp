#pragma once

namespace Tools
{
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    constexpr force_inline void combine_scalars(
        cref<a_T> a, cref<x_T> x, cref<b_T> b, mref<y_T> y
    )
    {
        // Computes y = a * opx( x ) + b * opy( y ).
        
        // We use references for the arguments in the case that
        // this would require to dereference some illegal pointer
        // upstream.
        
        static_assert( NotTransposedQ(opx),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opx."
        );
        
        static_assert( NotTransposedQ(opy),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opy."
        );
        
        static_assert( 
            Scalar::ComplexQ<y_T>
            ||
            Scalar::RealQ<decltype( Scalar::Op<a_flag,opx>( a_T(0), x_T(0) ) )>
            ,
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `a` and `b` to the right precision because this is better done once before any loop that calls `combine_scalars`. Hence we prefer a compile error here.
        
        static_assert(
            Scalar::Prec<a_T> == Scalar::Prec<x_T>,
            "Precisions of first and second argument do not coincide."
        );
        
        static_assert(
            Scalar::Prec<b_T> == Scalar::Prec<y_T>,
            "Precisions of third and fourth argument do not coincide."
        );

        
        auto a_opx_x = Scalar::Op<a_flag,opx>(a,x);
        auto b_opy_y = Scalar::Op<b_flag,opy>(b,y);
        
        y = scalar_cast<decltype(b_opy_y)>(a_opx_x) + b_opy_y;
    }
    
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T, typename b_T, typename y_T, typename z_T
    >
    constexpr force_inline void combine_scalars(
        cref<a_T> a, cref<x_T> x, cref<b_T> b, cref<y_T> y, mref<z_T> z
    )
    {
        // This computes z = a * opx( x ) + b * opy( y ).
        
        // We use references for the arguments in the case that
        // this would require to dereference some illegal pointer
        // upstream.
        
        static_assert( NotTransposedQ(opx),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opx."
        );
        
        static_assert( NotTransposedQ(opy),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opy."
        );
        
        static_assert(
            Scalar::ComplexQ<z_T>
            ||
            Scalar::RealQ<decltype( Scalar::Op<a_flag,opx>( a_T(0), x_T(0) ) + Scalar::Op<b_flag,opy>( b_T(0), y_T(0) ) )>
            ,
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `a` and `b` to the right precision because this is better done once before any loop that calls `combine_scalars`. Hence we prefer a compile error here.
        
        static_assert(
            Scalar::Prec<a_T> == Scalar::Prec<z_T>,
            "Precisions of first and last argument do not coincide."
        );
        
        static_assert(
            Scalar::Prec<b_T> == Scalar::Prec<z_T>,
            "Precisions of third and last argument do not coincide."
        );
        
        auto a_opx_x = Scalar::Op<a_flag,opx>(a,x);
        auto b_opy_y = Scalar::Op<b_flag,opy>(b,y);
        
        z = scalar_cast<z_T>(a_opx_x) + scalar_cast<z_T>(b_opy_y);
    }
    
} // namespace Tools




//#pragma once
//
//namespace Tools
//{
//    template<
//        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
//        Op opx = Op::Id, Op opy = Op::Id,
//        typename a_T, typename x_T,
//        typename b_T, typename y_T
//    >
//    constexpr force_inline void combine_scalars(
//        const a_T alpha, cref<x_T> x,
//        const b_T beta , mref<y_T> y
//    )
//    {
//        // Computes y = alpha * opx( x ) + beta * opy( y ).
//        
//        
//        static_assert( (opx == Op::Id) || (opx == Op::Conj),
//            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opx."
//        );
//        
//        static_assert( (opy == Op::Id) || (opy == Op::Conj),
//            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opy."
//        );
//        
//        constexpr auto ox = ( opx == Op::Conj ?
//                []( const x_T & X ){ return scalar_cast<y_T>( Conj<x_T>(X) ); } :
//                []( const x_T & X ){ return scalar_cast<y_T>( X ); }
//        );
//        
//        constexpr auto oy = ( opy == Op::Conj ?
//                []( const y_T & Y ){ return Conj<y_T>(Y); } :
//                []( const y_T & Y ){ return Y; }
//        );
//        
//        static_assert( Scalar::ComplexQ<y_T> || (Scalar::RealQ<a_T> && Scalar::RealQ<x_T> && Scalar::RealQ<b_T>),
//            "Fourth argument is real, but some of the other arguments are complex."
//        );
//        
//        // We refrain from automagically casting `alpha` and `beta` to the right precision because this is better done once before any loop that calls `combine_scalars`. Hence we prefer a compile error here.
//        
//        static_assert(
//            Scalar::Prec<a_T> == Scalar::Prec<y_T>,
//            "Precisions of first and fourth argument do not coincide."
//        );
//        
//        static_assert(
//            Scalar::Prec<b_T> == Scalar::Prec<y_T>,
//            "Precisions of third and fourth argument do not coincide."
//        );
//        
//        switch( alpha_flag )
//        {
//            case Scalar::Flag::Generic:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        y = alpha * ox(x) + beta * oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        y = alpha * ox(x) + oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        y = alpha * ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        y = alpha * ox(x) - oy(y);
//                        break;
//                    }
//                }
//                break;
//            }
//            case Scalar::Flag::Plus:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        y = ox(x) + beta * oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        y = ox(x) + oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        y = ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        y = ox(x) - oy(y);
//                        break;
//                    }
//                }
//                break;
//            }
//            case Scalar::Flag::Zero:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        y = beta * oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        y = oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        y = Scalar::Zero<y_T>;
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        y = -oy(y);
//                        break;
//                    }
//                }
//                break;
//            }
//            case Scalar::Flag::Minus:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        y = beta * oy(y) - ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        y = oy(y) - ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        y = - ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        y = -( ox(x) + oy(y) );
//                        break;
//                    }
//                }
//                break;
//            }
//        }
//    }
//    
//    
//    template<
//        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
//        Op opx = Op::Id, Op opy = Op::Id,
//        typename a_T, typename x_T,
//        typename b_T, typename y_T, typename z_T
//    >
//    constexpr force_inline void combine_scalars(
//        const a_T alpha, cref<x_T> x,
//        const b_T beta,  cref<y_T> y,
//                         mref<z_T> z
//    )
//    {
//        // This computes z = alpha * opx( x ) + beta * opy( y ).
//        // Most importantly, we guarantee casting to the correct output type.
//        
//        
//        static_assert( (opx == Op::Id) || (opx == Op::Conj),
//            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opx."
//        );
//        
//        static_assert( (opy == Op::Id) || (opy == Op::Conj),
//            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opy."
//        );
//        
//        constexpr auto ox = ( opx == Op::Conj ?
//                []( const x_T & X ){ return scalar_cast<z_T>( Conj<x_T>(X) ); } :
//                []( const x_T & X ){ return scalar_cast<z_T>( X ); }
//        );
//        
//        constexpr auto oy = ( opy == Op::Conj ?
//                []( const y_T & Y ){ return scalar_cast<z_T>( Conj<y_T>(Y) ); } :
//                []( const y_T & Y ){ return scalar_cast<z_T>( Y ); }
//        );
//        
//        static_assert( Scalar::ComplexQ<z_T> || (Scalar::RealQ<a_T> && Scalar::RealQ<x_T> && Scalar::RealQ<b_T> && Scalar::RealQ<y_T>),
//            "Last argument is real, but some of the other arguments are complex."
//        );
//        
//        // We refrain from automagically casting `alpha` and `beta` to the right precision because this is better done once before any loop that calls `combine_scalars`. Hence we prefer a compile error here.
//        
//        static_assert(
//            Scalar::Prec<a_T> == Scalar::Prec<z_T>,
//            "Precisions of first and last argument do not coincide."
//        );
//        
//        static_assert(
//            Scalar::Prec<b_T> == Scalar::Prec<z_T>,
//            "Precisions of third and last argument do not coincide."
//        );
//        
//        switch( alpha_flag )
//        {
//            case Scalar::Flag::Generic:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        z = alpha * ox(x) + beta * oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        z = alpha * ox(x) + oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        z = alpha * ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        z = alpha * ox(x) - oy(y);
//                        break;
//                    }
//                }
//                break;
//            }
//            case Scalar::Flag::Plus:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        z = ox(x) + beta * oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        z = ox(x) + oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        z = ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        z = ox(x) - oy(y);
//                        break;
//                    }
//                }
//                break;
//            }
//            case Scalar::Flag::Zero:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        z = beta * oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        z = oy(y);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        z = Scalar::Zero<z_T>;
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        z = - oy(y);
//                        break;
//                    }
//                }
//                break;
//            }
//            case Scalar::Flag::Minus:
//            {
//                switch( beta_flag )
//                {
//                    case Scalar::Flag::Generic:
//                    {
//                        z = beta * oy(y) - ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Plus:
//                    {
//                        z = oy(y) - ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Zero:
//                    {
//                        z = - ox(x);
//                        break;
//                    }
//                    case Scalar::Flag::Minus:
//                    {
//                        z = -( ox(x) + oy(y) );
//                        break;
//                    }
//                }
//                break;
//            }
//        }
//    }
//    
//} // namespace Tools
