#pragma once

namespace Tools
{
    namespace Scalar
    {
        
//##########################################################################
//##  Complex-Real
//##########################################################################
    
        template<Flag a_flag, Tools::Op op, typename R>
        force_inline constexpr
        std::enable_if_t<
            VectorizableQ<R> && OpReturnRealQ<a_flag,op,R,Cplx<R>>(),
            R
        >
        Op_vec( cref<Cplx<R>> a, cref<R> x )
        {
            if constexpr ( (a_flag == Flag::Zero) || (op == Tools::Op::Im) )
            {
                return 0;
            }
            else if constexpr ( (a_flag == Flag::Plus) )
            {
                return x;
            }
            else if constexpr ( (a_flag == Flag::Minus) )
            {
                return -x;
            }
            else
            {
                static_assert(false,"");
            }
        }
        
        template<Flag a_flag, Tools::Op op, typename R>
        force_inline constexpr
        std::enable_if_t<
            VectorizableQ<R> && !OpReturnRealQ<a_flag,op,R,Cplx<R>>(),
            vec_T<2,R>
        >
        Op_vec( cref<Cplx<R>> a, cref<R> x )
        {
            using V_T = vec_T<2,R>;
            
            if constexpr ( a_flag == Flag::Generic )
            {
                V_T & A = *reinterpret_cast<const V_T *>(&a);
                return A * x;
            }
            else
            {
                static_assert(false,"");
            }
        }
    
//##########################################################################
//##  Real-Complex
//##########################################################################
    
        template<Flag a_flag, Tools::Op op, typename R>
        force_inline constexpr
        std::enable_if_t<
            VectorizableQ<R> && OpReturnRealQ<a_flag,op,R,Cplx<R>>(),
            R
        >
        Op_vec( cref<R> a, cref<Cplx<R>> x )
        {
            if constexpr ( a_flag == Flag::Zero )
            {
                return 0;
            }
            else if constexpr ( (op == Tools::Op::Re) || (op == Tools::Op::ReTrans) )
            {
                return Op<a_flag,Tools::Op::Id>(a,Re(x));
            }
            else if constexpr ( (op == Tools::Op::Im) || (op == Tools::Op::ImTrans) )
            {
                return Op<a_flag,Tools::Op::Id>(a,Im(x));
            }
            else
            {
                static_assert(false,"");
            }
        }

        template<Flag a_flag, Tools::Op op, typename R>
        force_inline constexpr
        std::enable_if_t<
            VectorizableQ<R> && !OpReturnRealQ<a_flag,op,R,Cplx<R>>(),
            vec_T<2,R>
        >
        Op_vec( cref<R> a, cref<Cplx<R>> x )
        {
            using V_T = vec_T<2,R>;
            
            const V_T & X = *reinterpret_cast<const V_T * >(&x);
            
            if constexpr ( a_flag == Flag::Plus )
            {
                if constexpr ( op == Tools::Op::Id )
                {
                    return X;
                }
                else if constexpr ( op == Tools::Op::Conj )
                {
                    return V_T{Re(x),-Im(x)};
                }
                else
                {
                    static_assert(false,"");
                }
            }
            else if constexpr ( a_flag == Flag::Minus )
            {
                if constexpr ( op == Tools::Op::Id )
                {
                    return -X;
                }
                else if constexpr ( op == Tools::Op::Conj )
                {
                }
                else
                {
                    static_assert(false,"");
                }
            }
            else if constexpr ( a_flag == Flag::Generic )
            {
                if constexpr ( op == Tools::Op::Id )
                {
                    V_T A_0 {  Re(a),  Im(a) };
                    V_T A_1 { -Im(a),  Re(a) };
                    
                    return A_0 * Re(x) + A_1 * Im(x);
                }
                else if constexpr ( op == Tools::Op::Conj )
                {
                    V_T A_0 {  Re(a),  Im(a) };
                    V_T A_1 {  Im(a), -Re(a) };
                    
                    return A_0 * Re(x) + A_1 * Im(x);
                }
                else
                {
                    static_assert(false,"");
                }
            }
            else
            {
                static_assert(false,"");
            }
        }
    
//##########################################################################
//##  Complex-Complex
//##########################################################################
    
        template<Flag a_flag, Tools::Op op, typename R>
        force_inline constexpr
        std::enable_if_t<
            VectorizableQ<R> && OpReturnRealQ<a_flag,op,Cplx<R>,Cplx<R>>(),
            R
        >
        Op_vec( cref<Cplx<R>> a, cref<Cplx<R>> x )
        {
            if constexpr ( (a_flag == Flag::Zero) )
            {
                return 0;
            }
            else if constexpr ( (op == Tools::Op::Re) || (op == Tools::Op::ReTrans) )
            {
                return Op_vec<a_flag,Tools::Op::Id>(a,Re(x));
            }
            else if constexpr ( (op == Tools::Op::Im) || (op == Tools::Op::ImTrans) )
            {
                return Op_vec<a_flag,Tools::Op::Id>(a,Im(x));
            }
            else
            {
                static_assert(false,"");
            }
        }

        template<Flag a_flag, Tools::Op op, typename R>
        force_inline constexpr
        std::enable_if_t<
            VectorizableQ<R> && !OpReturnRealQ<a_flag,op,Cplx<R>,Cplx<R>>(),
            vec_T<2,R>
        >
        Op_vec( cref<Cplx<R>> a, cref<Cplx<R>> x )
        {
            using V_T = vec_T<2,R>;
            
            if constexpr ( (op == Tools::Op::Re) || (op == Tools::Op::ReTrans) )
            {
                return Op_vec<a_flag,Tools::Op::Id>(a,Re(x));
            }
            else if constexpr ( (op == Tools::Op::Im) || (op == Tools::Op::ImTrans) )
            {
                return Op_vec<a_flag,Tools::Op::Id>(a,Im(x));
            }
            else if constexpr ( a_flag == Flag::Plus )
            {
                return Op_vec<Flag::Plus,op>(R(1),x);
            }
            else if constexpr ( a_flag == Flag::Minus )
            {
                return Op_vec<Flag::Minus,op>(R(-1),x);
            }
            else if constexpr ( a_flag == Flag::Generic )
            {
                if constexpr( op == Tools::Op::Id )
                {
                    V_T A_0 {  Re(a),  Im(a) };
                    V_T A_1 { -Im(a),  Re(a) };
                    
                    return A_0 * Re(x) + A_1 * Im(x);
                }
                else if constexpr( op == Tools::Op::Conj )
                {
                    V_T A_0 {  Re(a),  Im(a) };
                    V_T A_1 {  Im(a), -Re(a) };
                    
                    return A_0 * Re(x) + A_1 * Im(x);
                }
                else
                {
                    static_assert(false,"");
                }
            }
            else
            {
                static_assert(false,"");
            }
        }

    } // namespace Scalar
        
} // namespace Tools
