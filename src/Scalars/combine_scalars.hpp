#pragma once

namespace Tools
{
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T,
        typename b_T, typename y_T
    >
    constexpr force_inline void combine_scalars(
        const a_T a, cref<x_T> x,
        const b_T b, mref<y_T> y
    )
    {
        // Computes y = a * opx( x ) + b * opy( y ).
        
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        constexpr auto ox = ( opx == Op::Conj ?
                []( const x_T & X ){ return scalar_cast<y_T>( Conj<x_T>(X) ); } :
                []( const x_T & X ){ return scalar_cast<y_T>( X ); }
        );
        
        constexpr auto oy = ( opy == Op::Conj ?
                []( const y_T & Y ){ return Conj<y_T>(Y); } :
                []( const y_T & Y ){ return Y; }
        );
        
        static_assert( Scalar::ComplexQ<y_T> || (Scalar::RealQ<a_T> && Scalar::RealQ<x_T> && Scalar::RealQ<b_T>),
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `a` and `b` to the right precision because this is better done once before any loop that calls `combine_scalars`. Hence we prefer a compile error here.
        
        static_assert(
            Scalar::Prec<a_T> == Scalar::Prec<y_T>,
            "Precisions of first and fourth argument do not coincide."
        );
        
        static_assert(
            Scalar::Prec<b_T> == Scalar::Prec<y_T>,
            "Precisions of third and fourth argument do not coincide."
        );
        
        switch( a_flag )
        {
            case Scalar::Flag::Generic:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = a * ox(x) + b * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = a * ox(x) + oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = a * ox(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = a * ox(x) - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Plus:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = ox(x) + b * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = ox(x) + oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = ox(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = ox(x) - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Zero:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = b * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = Scalar::Zero<y_T>;
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = -oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Minus:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = b * oy(y) - ox(x);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = oy(y) - ox(x);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = - ox(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = -( ox(x) + oy(y) );
                        break;
                    }
                }
                break;
            }
        }
    }
    
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag,
        Op opx = Op::Id, Op opy = Op::Id,
        typename a_T, typename x_T,
        typename b_T, typename y_T, typename z_T
    >
    constexpr force_inline void combine_scalars(
        const a_T a, cref<x_T> x,
        const b_T b, cref<y_T> y,
                     mref<z_T> z
    )
    {
        // This computes z = a * opx( x ) + b * opy( y ).
        // Most importantly, we guarantee casting to the correct output type.
        
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        constexpr auto ox = ( opx == Op::Conj ?
                []( const x_T & X ){ return scalar_cast<z_T>( Conj<x_T>(X) ); } :
                []( const x_T & X ){ return scalar_cast<z_T>( X ); }
        );
        
        constexpr auto oy = ( opy == Op::Conj ?
                []( const y_T & Y ){ return scalar_cast<z_T>( Conj<y_T>(Y) ); } :
                []( const y_T & Y ){ return scalar_cast<z_T>( Y ); }
        );
        
        static_assert( Scalar::ComplexQ<z_T> || (Scalar::RealQ<a_T> && Scalar::RealQ<x_T> && Scalar::RealQ<b_T> && Scalar::RealQ<y_T>),
            "Last argument is real, but some of the other arguments are complex."
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
        
        switch( a_flag )
        {
            case Scalar::Flag::Generic:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        z = a * ox(x) + b * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        z = a * ox(x) + oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        z = a * ox(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        z = a * ox(x) - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Plus:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        z = ox(x) + b * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        z = ox(x) + oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        z = ox(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        z = ox(x) - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Zero:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        z = b * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        z = oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        z = Scalar::Zero<z_T>;
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        z = - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Minus:
            {
                switch( b_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        z = b * oy(y) - ox(x);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        z = oy(y) - ox(x);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        z = - ox(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        z = -( ox(x) + oy(y) );
                        break;
                    }
                }
                break;
            }
        }
    }
    
} // namespace Tools
