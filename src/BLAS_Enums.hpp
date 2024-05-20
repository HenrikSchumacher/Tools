#pragma once

// It is somewhat unorthodox to use the Tensors namespace here, but I need some of these enums also for BLAS 1 routines.

namespace Tensors
{
    // cf. CBLAS_TRANSPOSE
    enum class Op: unsigned char
    {
        Id           = 111,
        Trans        = 112,
        ConjTrans    = 113,
        Conj         = 114
    };
    
    constexpr bool TransposedQ( const Op op )
    {
        return (op == Op::Trans) || (op == Op::ConjTrans);
    }
    
    constexpr bool NotTransposedQ( const Op op )
    {
        return (op == Op::Id) || (op == Op::Conj);
    }
    
    constexpr bool ConjugatedQ( const Op op )
    {
        return (op == Op::Conj) || (op == Op::ConjTrans);
    }
    
    template<Tools::Scalar::Flag a_flag, Op opx, typename S, typename T, typename R = decltype( S(1) * T(1) ) >
    force_inline constexpr R ScalarOperator( const S a, const T x )
    {
        if constexpr ( (opx == Op::Id) || (opx == Op::Trans) )
        {
            return Mult<a_flag>(a,x);
        }
        else
        {
            return Mult<a_flag>(a,Conj(x));
        }
    }
    
    
    [[nodiscard]] constexpr Op Transpose( const Op op )
    {
        switch( op )
        {
            case Tensors::Op::Id:        return Tensors::Op::Trans;
                
            case Tensors::Op::Conj:      return Tensors::Op::ConjTrans;
                
            case Tensors::Op::Trans:     return Tensors::Op::Id;
                
            case Tensors::Op::ConjTrans: return Tensors::Op::Conj;
        }
    }
    
    [[nodiscard]] constexpr Op Conjugate( const Op op )
    {
        switch( op )
        {
            case Tensors::Op::Id:        return Tensors::Op::Conj;
                
            case Tensors::Op::Conj:      return Tensors::Op::Id;
                
            case Tensors::Op::Trans:     return Tensors::Op::ConjTrans;
                
            case Tensors::Op::ConjTrans: return Tensors::Op::Trans;
        }
    }
    
    // cf. CBLAS_LAYOUT
    enum class Layout : unsigned char
    {
        RowMajor = 101,
        ColMajor = 102
    };
    
    
    // cf. CBLAS_UPLO
    enum class UpLo : unsigned char
    {
        Upper = 121,
        Lower = 122
    };
    
    // cf. CBLAS_DIAG
    enum class Diag : unsigned char
    {
        NonUnit = 131,
        Unit    = 132
    };
    
    // cf. CBLAS_SIDE
    enum class Side : unsigned char
    {
        Left  = 141,
        Right = 142
    };
    

    enum class AddTo_T : bool
    {
        True  = true,
        False = false
    };
    
    static constexpr AddTo_T AddTo     = AddTo_T::True;
    static constexpr AddTo_T Overwrite = AddTo_T::False;
}



namespace Tools
{
    [[nodiscard]] std::string ToString( const Tensors::Op op )
    {
        switch( op )
        {
            case Tensors::Op::Id:        return "Id";
                
            case Tensors::Op::Conj:      return "Conj";
                
            case Tensors::Op::Trans:     return "Trans";
                
            case Tensors::Op::ConjTrans: return "ConjTrans";
        }
    }

}
