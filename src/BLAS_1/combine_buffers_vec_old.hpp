#pragma once

namespace Tools
{
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        typename a_T, typename x_T, typename b_T, typename y_T
    >
    force_inline void combine_buffers_vec(
        cref<a_T> a_, cptr<x_T> x, cref<b_T> b_, mptr<y_T> y
    )
    {
        static_assert( VectorizableQ<y_T>, "combine_buffers_vec: type y_T must be allowable for clang's vector extension." );
        
        using namespace Scalar;
        
        using V = vec_T<N,y_T>;
        
        y_T a;
        y_T b;
        
        V x_vec;
        V y_vec;
        
        if constexpr ( a_flag != Flag::Zero  )
        {
            copy_buffer<N>( x, reinterpret_cast<y_T *>(&x_vec) );
            
            a = static_cast<y_T>(a_);
        }
        
        if constexpr (
            (b_flag != Flag::Zero)
            &&
            !( (a_flag == Flag::Zero) && ( b_flag == Flag::Plus ) ) )
        {
            copy_buffer<N>( y, reinterpret_cast<y_T *>(&y_vec) );
            
            b = static_cast<y_T>(b_);
        }
        
        if constexpr ( a_flag == Flag::Zero  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                zerofy_buffer<N>(y);
                return;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                return;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                y_vec *= -Scalar::One<y_T>;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                y_vec *= b;
            }
        }
        else if constexpr ( a_flag == Flag::Plus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                copy_buffer<N>(x,y);
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                y_vec += x_vec;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                y_vec = x_vec - y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                y_vec = x_vec + b * y_vec;
            }
        }
        else if constexpr ( a_flag == Flag::Minus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                y_vec = -x_vec;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                y_vec -= x_vec;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                y_vec = -x_vec - y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                y_vec = b * y_vec - x_vec ;
            }
        }
        else if constexpr ( a_flag == Flag::Generic  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                y_vec = a * x_vec;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                y_vec += a * x_vec;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                y_vec = a * x_vec - y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                y_vec = a * x_vec + b * y_vec;
            }
        }
        
        copy_buffer<N>( reinterpret_cast<y_T *>(&y_vec), y );
    }
    
    
    template<
        Scalar::Flag a_flag, Scalar::Flag b_flag, Size_T N,
        typename a_T, typename x_T, typename b_T, typename y_T, typename z_T
    >
    force_inline void combine_buffers_vec(
        cref<a_T> a_, cptr<x_T> x, cref<b_T> b_, cptr<y_T> y, mptr<z_T> z
    )
    {
        static_assert( VectorizableQ<z_T>, "combine_buffers_vec: type z_T be must allowable for clang's vector extension." );
        
        using namespace Scalar;
        
        using V = vec_T<N,z_T>;
        
        z_T a;
        z_T b;
        
        V x_vec;
        V y_vec;
        V z_vec;
        
        if constexpr ( a_flag != Flag::Zero  )
        {
            copy_buffer<N>( x, reinterpret_cast<z_T *>(&x_vec) );
            
            a = static_cast<z_T>(a_);
        }
        
        if constexpr ( b_flag != Flag::Zero  )
        {
            copy_buffer<N>( y, reinterpret_cast<z_T *>(&y_vec) );
            
            b = static_cast<z_T>(b_);
        }
        
        if constexpr ( a_flag == Flag::Zero  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
//                z_vec = Scalar::Zero<z_T>;
                zerofy_buffer<N>(z);
                return;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
//                z_vec = y_vec;
                
                copy_buffer<N>(y,z);
                return;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                z_vec = -y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                z_vec = b * y_vec;
            }
        }
        else if constexpr ( a_flag == Flag::Plus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
//                z_vec = x_vec;
                
                copy_buffer<N>(x,z);
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                z_vec = x_vec + y_vec;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                z_vec = x_vec - y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                z_vec = x_vec + b * y_vec;
            }
        }
        else if constexpr ( a_flag == Flag::Minus  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                z_vec = -x_vec;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                z_vec = y_vec - x_vec;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                z_vec = -x_vec - y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                z_vec = b * y_vec - x_vec ;
            }
        }
        else if constexpr ( a_flag == Flag::Generic  )
        {
            if constexpr ( b_flag == Flag::Zero  )
            {
                z_vec = a * x_vec;
            }
            else if constexpr ( b_flag == Flag::Plus  )
            {
                z_vec = a * x_vec + y_vec;
            }
            else if constexpr ( b_flag == Flag::Minus  )
            {
                z_vec = a * x_vec - y_vec;
            }
            else if constexpr ( b_flag == Flag::Generic  )
            {
                z_vec = a * x_vec + b * y_vec;
            }
        }
        
        copy_buffer<N>( reinterpret_cast<z_T *>(&z_vec), z );
    }

} // namespace Tools
