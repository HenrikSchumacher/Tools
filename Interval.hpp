#pragma once

// Beware: At compile time all floating point rounding is towards nearest!!!

#define CHECK_ROUNDING_MODE
#define INTERVAL_DEBUG


#include <cfenv>

#include "Tools.hpp"

// TODO: Read this:
// https://www.codeproject.com/Articles/1040839/Interval-arithmetic-using-round-to-nearest-mode-pa


// TODO: square
// TODO: integer power.

namespace Tools
{
    template<typename Real_>
    class Interval
    {
        ASSERT_FLOAT(Real_)
        
    public:
        
        using Real = Real_;
        
    protected:
        
        Real a;
        Real b;
        
    public:
        
        Interval()
        : a{ 0 }
        , b{ 0 }
        {}
        
        explicit Interval( const Real & x )
        : a{ -x }
        , b{  x }
        {}
        
        explicit Interval( const Real & lower_, const Real & upper_ )
//        : a{ -a_ }
//        , b{  b_ }
        : a{ -Min(lower_,upper_) }
        , b{  Max(lower_,upper_) }
        {}
        
        ~Interval() = default;
        
    
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
        
        explicit Interval( cref<mma::TensorRef<Real>> A )
        : Interval{ A.data()[0], A.data()[1]  }
        {}
        
        mma::TensorRef<Real> to_MTensorRef() const
        {
            mma::makeVector<Real>( 2 ) R;
            
            data(R)[0] = -a;
            data(R)[1] =  b;
            
            return R;
        }
        
        
#endif
        
    
        
        // Copy constructor
        Interval( const Interval & J )
        :   a { J.a }
        ,   b { J.b }
        {}
        
        
        static Interval Create( const Real & a, const Real & b )
        {
#if defined(INTERVAL_DEBUG)
            if( -a > b )
            {
                eprint("Create: -a > b");
                dump(-a);
                dump(b);
            }
#endif
            Interval K;
            K.a = a;
            K.b = b;
            
            return K;
        }
        
        friend void swap (Interval & I, Interval & J ) noexcept
        {
            // see https://stackoverflow.com/questions/5695548/public-friend-swap-member-function for details
            using std::swap;
            
            swap( I.a, J.a );
            swap( I.b, J.b );
        }
        
        // (Copy-)assignment operator
        Interval & operator=( Interval other )
        {
            swap( *this, other );
            
            return *this;
        }
        
        // Move constructor
        Interval( Interval && other ) noexcept
        :   Interval()
        {
            swap(*this, other);
        }
//        
//        // (Move-)assignment operator
//        Interval & operator=( Interval && other )
//        {
//            std::swap(*this, other);
//            
//            return( *this );
//        }
      
    public:
        
        Real Upper() const
        {
            return b;
        }
        
        Real Lower() const
        {
            return -a;
        }

        force_inline bool SingletonQ() const
        {
            return (-a==b);
        }
        
        force_inline bool ContainsZeroQ() const
        {
            return (b >= 0) && (a >= 0);
        }
        
        force_inline bool ContainsQ( cref<Real> x ) const
        {
            return (b >= x) && (-a <= x);
        }
        
        force_inline bool NotContainsQ( cref<Real> x ) const
        {
            return (b < x) || (-a > x);
        }
        
        force_inline bool FullyNegativeQ() const
        {
            return b < 0;
        }
        
        force_inline bool FullyPositiveQ() const
        {
            return a < 0;
        }
        
        //    TODO: operator+= (Interval)
        //    TODO: operator-= (Interval)
        //    TODO: operator*= (Interval)
        //    TODO: operator/= (Interval)
        
        //    TODO: operator+= (Real)
        //    TODO: operator-= (Real)
        //    TODO: operator*= (Real)
        //    TODO: operator/= (Real)
        
        
        
        static void RoundingModeWarning( cref<std::string> tag )
        {
            #ifdef CHECK_ROUNDING_MODE
          
            if( std::fegetround() != FE_UPWARD )
            {
                eprint(tag + " for Interval<"+TypeName<Real>+">: Rounding mode not set to FE_UPWARD.");
            }
            #endif
        }
        
        force_inline friend Interval operator+( cref<Interval> I, cref<Interval> J )
        {
            RoundingModeWarning("operator+");
            
            return Create( I.a + J.a, I.b + J.b );
        }
        
        force_inline friend Interval operator+( cref<Interval> I, cref<Real> x )
        {
            RoundingModeWarning("operator+");
            
            return Create( I.a - x, I.b + x );
        }
        
        force_inline friend Interval operator+( cref<Real> x, cref<Interval> I )
        {
            return I + x;
        }
        
        force_inline friend Interval operator-( cref<Interval> I, cref<Interval> J )
        {
            RoundingModeWarning("operator-");
            
            return Create( I.a + J.b , I.b + J.a );
        }
        
        force_inline friend Interval operator-( cref<Interval> I, cref<Real> x )
        {
            RoundingModeWarning("operator-");
            
            return Create( I.a + x.b , I.b - x );
        }
        
        force_inline friend Interval operator*( cref<Interval> I, cref<Interval> J )
        {
            RoundingModeWarning("operator*");
            
            // TODO: Eight multiplications? Are you serious?
            const Real A = Max(
                Max( (-I.a) * ( J.a), ( I.a) * ( J.b) ),
                Max( ( I.b) * ( J.a), (-I.b) * ( J.b) )
            );
            
            const Real B = Max(
                Max( ( I.a) * ( J.a), (-I.a) * ( J.b) ),
                Max( ( I.b) * (-J.a), ( I.b) * ( J.b) )
            );
            
            return Create( A, B );
        }
        
        force_inline friend Interval operator*( cref<Interval> I, cref<Real> x )
        {
            RoundingModeWarning("operator*");
            
            return Create(
                Max( ( I.a) * x, (-I.b) * x ),
                Max( (-I.a) * x, ( I.b) * x )
            );
        }
        
        force_inline friend Interval operator*( cref<Real> x, cref<Interval> I )
        {
            return I * x;
        }
        
        force_inline friend Interval fma(
            cref<Real>     factor,
            cref<Interval> I,
            cref<Real>     summand
        )
        {
            RoundingModeWarning("fma*");
            
            return Create (
                Max( std::fma(  I.a, factor, -summand ), std::fma( -I.b, factor, -summand ) ),
                Max( std::fma( -I.a, factor,  summand ), std::fma(  I.b, factor,  summand ) )
            );
        }
        
        force_inline friend Interval Abs( Interval & I )
        {
            const Real A = Abs(I.a);
            const Real B = Abs(I.b);
            
            return Create( I.ContainsQ() ? 0 : Min(A,B), Max(A,B) );
        }
        
        force_inline friend Interval Max( cref<Interval> I, cref<Interval> J )
        {
            return Create( Min(I.a,J.a), Max(I.b,J.b) );
        }
        
        force_inline friend Interval Min( cref<Interval> I, cref<Interval> J )
        {
            return Create( Max(I.a,J.a), Min(I.b,J.b) );
        }
        
        
        force_inline friend bool operator<( cref<Interval> I, cref<Real> x )
        {
            return I.b < x;
        }
        
        force_inline friend bool operator>( cref<Real> x, cref<Interval> I )
        {
            return I < x;
        }
        
        force_inline friend bool operator>( cref<Interval> I, cref<Real> x )
        {
            return -I.a > x;
        }
        
        force_inline friend bool operator<( cref<Real> x, cref<Interval> I )
        {
            return I > x;
        }
        
        Real Width() const
        {
            return (a + b);
        }
        
        Real EpsRelativeWidth() const
        {
            return ( (a + b) / Max(Abs(a),Abs(b)) ) / Scalar::eps<Real>;
        }
        
    }; // class Interval
    

    template<typename T>
    std::string ToString( const Interval<T> & I )
    {
        return std::string("[ ") + Tools::ToString(I.Lower()) + ", " + Tools::ToString(I.Upper()) + " ]";
    }



    
    
    

    

    


//    TODO: operator<
//    TODO: operator>
    
//    TODO: operator+
//    TODO: operator-
//    TODO: operator*
//    TODO: operator/
    
//    TODO: Max
//    TODO: Min
//    TODO: Ramp

//    TODO: Things from the Scalar namespace, e.g., RealQ, ComplexQ, Prec etc.
//    TODO: constants! (e.g. Pi)

    
}
