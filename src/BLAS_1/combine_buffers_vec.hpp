#pragma once

namespace Tools
{
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        typename Scal
    >
    force_inline constexpr
    std::enable_if_t<VectorizableQ<Scal>,void> combine_buffers_vec(
        cref<Scal> a, const Scal * x, cref<Scal> b, Scal * y
    )
    {
        using namespace Scalar;
        
        using V = vec_T<N,Scal>;
        
        const V & X = *reinterpret_cast<const V *>(x);
              V & Y = *reinterpret_cast<      V *>(y);
        
        if constexpr ( a_flag == Flag::Zero  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Y = 0;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                // Do nothing.
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Y = -Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Y *= b;
            }
        }
        else if constexpr ( a_flag == Flag::Plus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Y = X;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Y += X;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Y = X - Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Y = X + b * Y;
            }
        }
        else if constexpr ( a_flag == Flag::Minus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Y = -X;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Y = Y - X;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Y = -X - Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Y = b * Y - X ;
            }
        }
        else if constexpr ( a_flag == Flag::Generic  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Y = a * X;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Y += a * X;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Y = a * X - Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Y = a * X + b * Y;
            }
        }
    }
    
    
//    template<
//        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
//        typename Scal
//    >
//    force_inline constexpr std::enable_if_t<VectorizableQ<Scal>,void>
//    combine_buffers_vec(
//        cref<Scal> a, const Scal * x, cref<Scal> b, const Scal * y, Scal * z
//    )
//    {
//        using namespace Scalar;
//        
//        using V = vec_T<N,Scal>;
//        
//        const V & X = *reinterpret_cast<const V *>(x);
//        const V & Y = *reinterpret_cast<const V *>(y);
//              V & Z = *reinterpret_cast<      V *>(z);
//        
//        if constexpr ( a_flag == Flag::Zero  )
//        {
//            if constexpr ( b_flag == Flag::Zero  )
//            {
//                Z = 0;
//            }
//            else if constexpr ( b_flag == Flag::Plus  )
//            {
//                Z = Y;
//            }
//            else if constexpr ( b_flag == Flag::Minus  )
//            {
//                Z = -Y;
//            }
//            else if constexpr ( b_flag == Flag::Generic  )
//            {
//                Z = b * Y;
//            }
//        }
//        else if constexpr ( a_flag == Flag::Plus  )
//        {
//            if constexpr ( b_flag == Flag::Zero  )
//            {
//                Z = X;
//            }
//            else if constexpr ( b_flag == Flag::Plus  )
//            {
//                Z = X + Y;
//            }
//            else if constexpr ( b_flag == Flag::Minus  )
//            {
//                Z = X - Y;
//            }
//            else if constexpr ( b_flag == Flag::Generic  )
//            {
//                Z = X + b * Y;
//            }
//        }
//        else if constexpr ( a_flag == Flag::Minus  )
//        {
//            if constexpr ( b_flag == Flag::Zero  )
//            {
//                Z = -X;
//            }
//            else if constexpr ( b_flag == Flag::Plus  )
//            {
//                Z = Y - X;
//            }
//            else if constexpr ( b_flag == Flag::Minus  )
//            {
//                Z = -X - Y;
//            }
//            else if constexpr ( b_flag == Flag::Generic  )
//            {
//                Z = b * Y - X ;
//            }
//        }
//        else if constexpr ( a_flag == Flag::Generic  )
//        {
//            if constexpr ( b_flag == Flag::Zero  )
//            {
//                Z = a * X;
//            }
//            else if constexpr ( b_flag == Flag::Plus  )
//            {
//                Z = a * X + Y;
//            }
//            else if constexpr ( b_flag == Flag::Minus  )
//            {
//                Z = a * X - Y;
//            }
//            else if constexpr ( b_flag == Flag::Generic  )
//            {
//                Z = a * X + b * Y;
//            }
//        }
//    }
    
    // Experimental code
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        typename Scal
    >
    force_inline constexpr
    std::enable_if_t<VectorizableQ<Scal>,void> combine_buffers_vec(
        cref<Scal> a, const Scal * x, cref<Scal> b, const Scal * y, Scal * z
    )
    {
        using namespace Scalar;
        
        constexpr Size_T vec_size = 4;
        
        using V_T = vec_T<vec_size,Scal>;
        
        constexpr Size_T N_rounded = RoundDownTo(N,vec_size);
        
        for( Size_T i = 0; i < N_rounded; ++i )
        {
            const V_T & X = *reinterpret_cast<const V_T *>(&x[vec_size * i]);
            const V_T & Y = *reinterpret_cast<const V_T *>(&y[vec_size * i]);
                  V_T & Z = *reinterpret_cast<      V_T *>(&z[vec_size * i]);
            
            Z = a * X + b * Y;
        }
        
        for( Size_T i = N_rounded; i < N; ++i )
        {
            z[i] = a * x[i] + b * y[i];
        }
    }
    
    
    // Experimental code
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        Op opx = Op::Id, Op opy = Op::Id,
        typename R
    >
    force_inline constexpr
    std::enable_if_t<VectorizableQ<R>,void> combine_buffers_vec(
        cref<Cplx<R>> a, cptr<Cplx<R>> x,
        cref<Cplx<R>> b, cptr<Cplx<R>> y,
                         mptr<Cplx<R>> z
    )
    {
        using namespace Scalar;
        
        static_assert( NotTransposedQ(opx),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opx."
        );
        
        static_assert( NotTransposedQ(opy),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opy."
        );
        
        using V_T = vec_T<2,R>;
        
        {
            #pragma float_control(precise, off)
                                                
            for( Size_T i = 0; i < N; ++i )
            {
                V_T & Z = *reinterpret_cast<V_T *>(&z[i]);
                
                Z = Scalar::Op_vec<a_flag,opx>(a,x[i])
                    +
                    Scalar::Op_vec<b_flag,opy>(b,y[i]);
            }
        }
    }
    
    // Experimental code
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        Op opx = Op::Id, Op opy = Op::Id,
        typename R
    >
    force_inline constexpr
    std::enable_if_t<VectorizableQ<R>,void> combine_buffers_vec(
        cref<Cplx<R>> a, cptr<Cplx<R>> x,
        cref<Cplx<R>> b, mptr<Cplx<R>> y
    )
    {
        using namespace Scalar;
        
        static_assert( NotTransposedQ(opx),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opx."
        );
        
        static_assert( NotTransposedQ(opy),
            "combine_scalars: Only those values of Op that do not inlude transposition are allowed for template argument opy."
        );
        
        using V_T = vec_T<2,R>;
        
        {
            #pragma float_control(precise, off)
                                                
            for( Size_T i = 0; i < N; ++i )
            {
                V_T & Y = *reinterpret_cast<V_T *>(&y[i]);
                
                Y = Scalar::Op_vec<a_flag,opx>(a,x[i])
                    +
                    Scalar::Op_vec<b_flag,opy>(b,y[i]);
            }
        }
    }
    
} // namespace Tools
