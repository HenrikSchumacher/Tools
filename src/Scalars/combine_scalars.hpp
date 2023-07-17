#pragma once

namespace Tools
{
    template<
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        Tensors::Op opx = Tensors::Op::Id, Tensors::Op opy = Tensors::Op::Id,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    constexpr force_inline void combine_scalars(
        cref<R_0> alpha, cref<S_0> x,
        cref<R_1> beta,  mref<S_1> y
    )
    {
        using Op = Tensors::Op;
        
        // Computes y = alpha * opx( x ) + beta * opy( y ).
        
        
        static_assert( (opx == Op::Id) || (opx == Op::Conj),
            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opx."
        );
        
        static_assert( (opy == Op::Id) || (opy == Op::Conj),
            "combine_scalars: Only the values Op::Id and Op::Conj are allowed for opy."
        );
        
        constexpr auto ox = COND( opx == Op::Conj,
                []( const S_0 & z ){ return Scalar::Conj<S_0>(z); },
                []( const S_0 & z ){ return z; }
        );
        
        constexpr auto oy = COND( opy == Op::Conj,
                []( const S_1 & z ){ return Scalar::Conj<S_1>(z); },
                []( const S_1 & z ){ return z; }
        );
        
        static_assert( Scalar::ComplexQ<S_1> || (Scalar::RealQ<R_0> && Scalar::RealQ<S_0> && Scalar::RealQ<R_1>),
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
        // We refrain from automagically casting `alpha` and `beta` to the right precision because this is better done once before any loop that calls `combine_scalars`. Hence we prefer a compile error here.
        
        static_assert(
            Scalar::Prec<R_0> == Scalar::Prec<S_1>,
            "Precisions of first and fourth argument do not coincide."
        );
        
        static_assert(
            Scalar::Prec<R_1> == Scalar::Prec<S_1>,
            "Precisions of third and fourth argument do not coincide."
        );
        
        switch( alpha_flag )
        {
            case Scalar::Flag::Generic:
            {
                switch( beta_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = (alpha * scalar_cast<S_1>( ox(x) )) + (beta * oy(y) );
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = alpha * scalar_cast<S_1>( ox(x) ) + oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = alpha * scalar_cast<S_1>( ox(x) );
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = (alpha * scalar_cast<S_1>( ox(x) )) - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Plus:
            {
                switch( beta_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = scalar_cast<S_1>( ox(x) ) + (beta * oy(y));
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = scalar_cast<S_1>( ox(x) ) + oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = scalar_cast<S_1>( ox(x) );
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = scalar_cast<S_1>( ox(x) ) - oy(y);
                        break;
                    }
                }
                break;
            }
            case Scalar::Flag::Zero:
            {
                switch( beta_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = beta * oy(y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = oy(y);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = Scalar::Zero<S_1>;
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
                switch( beta_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = (beta * oy(y)) - scalar_cast<S_1>( ox(x) );
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y = oy(y) - scalar_cast<S_1>( ox(x) );
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = scalar_cast<S_1>( - ox(x) );
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = -( scalar_cast<S_1>(ox(x)) + oy(y) );
                        break;
                    }
                }
                break;
            }
        }
    }
    
} // namespace Tools
