#pragma once

namespace Tools
{
    template<
        Scalar::Flag alpha_flag, Scalar::Flag beta_flag,
        typename R_0, typename S_0,
        typename R_1, typename S_1
    >
    constexpr force_inline void combine_scalars(
        const R_0 & alpha, const S_0 & x,
        const R_1 & beta,        S_1 & y
    )
    {
        static_assert( Scalar::IsComplex<S_1> || (Scalar::IsReal<R_0> && Scalar::IsReal<S_0> && Scalar::IsReal<R_1>),
            "Fourth argument is real, but some of the other arguments are complex."
        );
        
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
                        y = (alpha * scalar_cast<S_1>(x)) + (beta * y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y += alpha * scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = alpha * scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = (alpha * scalar_cast<S_1>(x)) - y;
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
                        y = scalar_cast<S_1>(x) + (beta * y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y += scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = scalar_cast<S_1>(x) - y;
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
                        y *= beta;
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        // Do nothing.
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = Scalar::Zero<S_1>;
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = -y;
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
                        y = scalar_cast<S_1>(-x) + (beta * y);
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y -= scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = scalar_cast<S_1>(-x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        y = -(scalar_cast<S_1>(x) + y);
                        break;
                    }
                }
                break;
            }
        }
    }
    
} // namespace Tools
