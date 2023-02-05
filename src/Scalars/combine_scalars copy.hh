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
        using Real_R_0 = typename ScalarTraits<R_0>::Real;
        using Real_S_0 = typename ScalarTraits<S_0>::Real;
        
        using Real_R_1 = typename ScalarTraits<R_1>::Real;
        using Real_S_1 = typename ScalarTraits<S_1>::Real;

        
#define Re_alpha static_cast<Real_S_1>(reinterpret_cast<const Real_R_0*>(&alpha)[0])
#define Im_alpha static_cast<Real_S_1>(reinterpret_cast<const Real_R_0*>(&alpha)[1])
        
#define Re_x     static_cast<Real_S_1>(reinterpret_cast<const Real_S_0*>(&x)[0])
#define Im_x     static_cast<Real_S_1>(reinterpret_cast<const Real_S_0*>(&x)[1])

#define Re_beta  static_cast<Real_S_1>(reinterpret_cast<const Real_R_1*>(&beta)[0])
#define Im_beta  static_cast<Real_S_1>(reinterpret_cast<const Real_R_1*>(&beta)[1])
        
#define Re_y                           reinterpret_cast<      Real_S_1*>(&y)[0]
#define Im_y                           reinterpret_cast<      Real_S_1*>(&y)[1]

        
        switch( alpha_flag )
        {
            case Scalar::Flag::Generic:
            {
                switch( beta_flag )
                {
                    case Scalar::Flag::Generic:
                    {
                        y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) + scalar_cast<S_1>(beta) * y;
//                        if constexpr ( ScalarTraits<S_1>::IsReal )
//                        {
//                            y = static_cast<S_1>(alpha) * static_cast<S_1>(x) + static_cast<S_1>(beta) * y;
//                        }
//                        else
//                        {
//                            if constexpr ( ScalarTraits<R_1>::IsReal )
//                            {
//                                Re_y *= static_cast<Real_S_1>(beta);
//                                Im_y *= static_cast<Real_S_1>(beta);
//                            }
//                            else
//                            {
//                                Re_y = Re_beta * Re_y - Im_beta * Im_y;
//                                Im_y = Re_beta * Im_y + Im_beta * Re_y;
//                            }
//                            
//                            if constexpr ( ScalarTraits<R_0>::IsReal )
//                            {
//                                
//                                if constexpr ( ScalarTraits<S_0>::IsReal )
//                                {
//                                    Re_y += static_cast<Real_S_1>(alpha) * static_cast<Real_S_1>(x);
//                                }
//                                else
//                                {
//                                    Re_y += static_cast<Real_S_1>(alpha) * Re_x;
//                                    Im_y += static_cast<Real_S_1>(alpha) * Im_x;
//                                }
//                            }
//                            else
//                            {
//                                if constexpr ( ScalarTraits<S_0>::IsReal )
//                                {
//                                    Re_y += Re_alpha * static_cast<Real_S_1>(x);
//                                    Im_y += Im_alpha * static_cast<Real_S_1>(x);
//                                }
//                                else
//                                {
//                                    Re_y += Re_alpha * Re_x - Im_alpha * Im_x;
//                                    Im_y += Re_alpha * Im_x + Im_alpha * Re_x;
//                                }
//                            }
//                        }
//                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        y += scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x);
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
//                        y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) - y;
                        y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) + scalar_cast<S_1>(-1.) * y;
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
                        y = scalar_cast<S_1>(x) + scalar_cast<S_1>(beta) * y;
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
//                        y = scalar_cast<S_1>(x) + scalar_cast<S_1>(-1.) * y;
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
                        if constexpr ( ScalarTraits<S_1>::IsReal )
                        {
                            y *= static_cast<S_1>(beta);
                        }
                        else
                        {
                            if constexpr ( ScalarTraits<R_1>::IsReal )
                            {
                                Re_y *= static_cast<Real_S_1>(beta);
                                Im_y *= static_cast<Real_S_1>(beta);
                            }
                            else
                            {
                                Re_y = Re_beta * Re_y - Im_beta * Im_y;
                                Im_y = Re_beta * Im_y + Im_beta * Re_y;
                            }
                        }
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        // Do nothing.
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        if constexpr ( ScalarTraits<S_1>::IsReal)
                        {
                            y = static_cast<S_1>(0);
                        }
                        else
                        {
                            Re_y = static_cast<Real_S_1>(0);
                            Im_y = static_cast<Real_S_1>(0);
                        }
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        if constexpr ( ScalarTraits<S_1>::IsReal)
                        {
                            y = -y;
                        }
                        else
                        {
                            Re_y = - Re_y;
                            Im_y = - Im_y;
                        }
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
                        // y = scalar_cast<S_1>(beta) * y + scalar_cast<S_1>(-x);
                        if constexpr ( ScalarTraits<S_1>::IsReal )
                        {
                            y = static_cast<S_1>(beta) * y + static_cast<S_1>(-x);
                        }
                        else
                        {
                            if constexpr ( ScalarTraits<R_1>::IsReal )
                            {
                                Re_y *= static_cast<Real_S_1>(beta);
                                Im_y *= static_cast<Real_S_1>(beta);
                            }
                            
                            if constexpr ( ScalarTraits<S_0>::IsReal )
                            {
                                Re_y -= static_cast<Real_S_1>(x);
                            }
                            else
                            {
                                Re_y -= Re_x;
                                Im_y -= Im_x;
                            }
                        }
                        break;
                    }
                    case Scalar::Flag::Plus:
                    {
                        if constexpr ( ScalarTraits<S_1>::IsReal )
                        {
                            y -= static_cast<S_1>(x);
                        }
                        else
                        {
                            if constexpr ( ScalarTraits<S_0>::IsReal )
                            {
                                Re_y -= static_cast<Real_S_1>(x);
                            }
                            else
                            {
                                Re_y -= Re_x;
                                Im_y -= Im_x;
                            }
                        }
                        break;
                    }
                    case Scalar::Flag::Zero:
                    {
                        if constexpr ( ScalarTraits<S_1>::IsReal )
                        {
                            y = static_cast<S_1>(-x);
                        }
                        else
                        {
                            if constexpr ( ScalarTraits<S_0>::IsReal )
                            {
                                Re_y = static_cast<Real_S_1>(-x);
                                Im_y = static_cast<Real_S_1>(0);
                            }
                            else
                            {
                                Re_y = -Re_x;
                                Im_y = -Im_x;
                            }
                        }
                        break;
                    }
                    case Scalar::Flag::Minus:
                    {
                        if constexpr ( ScalarTraits<S_1>::IsReal )
                        {
                            y = - (scalar_cast<S_1>(x) + y);
                        }
                        else
                        {
                            if constexpr ( ScalarTraits<S_0>::IsReal )
                            {
                                Re_y = -( Re_y + static_cast<Real_S_1>(x));
                                Im_y = -( Im_y + static_cast<Real_S_1>(0));
                            }
                            else
                            {
                                Re_y = -(Re_y + Re_x);
                                Im_y = -(Im_y + Im_x);
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
        
//        if constexpr ( alpha_flag == Scalar::Flag::Plus )
//        {
//            if constexpr ( beta_flag == Scalar::Flag::Zero )
//            {
//                y = scalar_cast<S_1>(x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Plus )
//            {
//                y += scalar_cast<S_1>(x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Minus )
//            {
//                y = scalar_cast<S_1>(x) - y;
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Generic )
//            {
//                y = scalar_cast<S_1>(x) + scalar_cast<S_1>(beta) * y;
//            }
//        }
//        else if constexpr ( alpha_flag == Scalar::Flag::Minus )
//        {
//            if constexpr ( beta_flag == Scalar::Flag::Zero )
//            {
//                y = scalar_cast<S_1>(-x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Plus )
//            {
//                y -= scalar_cast<S_1>(x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Minus )
//            {
//                // DEBUGGING;
//                y = -y;
//                y += scalar_cast<S_1>(-x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Generic )
//            {
//                y = scalar_cast<S_1>(beta) * y + scalar_cast<S_1>(-x);
//            }
//        }
//        else if constexpr ( alpha_flag == Scalar::Flag::Zero )
//        {
//            if constexpr ( beta_flag == Scalar::Flag::Zero )
//            {
//                y = static_cast<S_1>(0);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Plus )
//            {
////                 Do nothing.
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Minus )
//            {
//                y = -y;
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Generic )
//            {
//                y *= scalar_cast<S_1>(beta);
//            }
//        }
//        else if constexpr ( alpha_flag == Scalar::Flag::Generic )
//        {
//            // TODO: This assumes that R_1 and S_1 are compatible.
//            // TODO: This will do the computation in better precisiono of alpha and x!!!
//
//            if constexpr ( beta_flag == Scalar::Flag::Zero )
//            {
//                y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Plus )
//            {
//                y += scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x);
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Minus )
//            {
//                y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) - y;
//            }
//            else if constexpr ( beta_flag == Scalar::Flag::Generic )
//            {
//                y = scalar_cast<S_1>(alpha) * scalar_cast<S_1>(x) + scalar_cast<S_1>(beta) * y;
//            }
//        }
        
        
#undef Re_x;
#undef Im_x;
        
#undef Re_y;
#undef Im_y;

#undef Re_alpha;
#undef Im_alpha;
        
#undef Re_beta;
#undef Im_beta;
    }
    
}

