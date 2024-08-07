#pragma once

namespace Tools
{
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        typename Scal
    >
    force_inline void combine_buffers_vec(
        cref<Scal> a, const Scal * x, cref<Scal> b, Scal * y
    )
    {
        static_assert( VectorizableQ<Scal>, "combine_buffers_vec: type Scal must be allowable for clang's vector extension." );
        
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
    
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        typename Scal
    >
    force_inline void combine_buffers_vec(
        cref<Scal> a, const Scal * x, cref<Scal> b, const Scal * y, Scal * z
    )
    {
        static_assert( VectorizableQ<Scal>, "combine_buffers_vec: type Scal be must allowable for clang's vector extension." );
        
        using namespace Scalar;
        
        using V = vec_T<N,Scal>;
        
        const V & X = reinterpret_cast<const V *>(x);
        const V & Y = reinterpret_cast<const V *>(y);
              V & Z = reinterpret_cast<      V *>(z);
        
        if constexpr ( a_flag == Flag::Zero  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Z = 0;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Z = Y;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Z = -Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Z = b * Y;
            }
        }
        else if constexpr ( a_flag == Flag::Plus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Z = X;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Z = X + Y;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Z = X - Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Z = X + b * Y;
            }
        }
        else if constexpr ( a_flag == Flag::Minus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Z = -X;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Z = Y - X;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Z = -X - Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Z = b * Y - X ;
            }
        }
        else if constexpr ( a_flag == Flag::Generic  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                Z = a * X;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                Z = a * X + Y;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                Z = a * X - Y;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                Z = a * X + b * Y;
            }
        }
    }

} // namespace Tools
