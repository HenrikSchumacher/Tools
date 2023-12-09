#pragma once

// Beware: At compile time all floating point rounding is towards nearest!!!

#include <cfenv>

#include "Tools.hpp"

// TODO: Read this:
// https://www.codeproject.com/Articles/1040839/Interval-arithmetic-using-round-to-nearest-mode-pa


// TODO: MinMax.
// TODO: integer power.
// TODO: operator/

// TODO: Things from the Scalar namespace, e.g., RealQ, ComplexQ, Prec etc.
// TODO: constants! (e.g. Pi)


namespace Tools
{

    // From Hida, Li, Bailey - Library for Double-Double and Quad-Double Arithmetic
    template<typename Real>
    std::pair<Real,Real> TwoSum( const Real a, const Real b )
    {
        ASSERT_FLOAT(Real);
        
        const Real s = a + b;
        const Real v = s - b;
        const Real e = (a - (s - v)) + (b-v);
        
        return std::pair(s,e);
        
    }
    
    // From Hida, Li, Bailey - Library for Double-Double and Quad-Double Arithmetic
    template<typename Real>
    std::pair<Real,Real> TwoProdFMA( const Real a, const Real b )
    {
        ASSERT_FLOAT(Real);
        
        const Real p = a * b;
        const Real e = std::fma(a,b,-p);
        
        return std::pair(p,e);
        
    }
    
    template<typename Real>
    int DetSign2D_Corrected( cptr<Real> A )
    {
        ASSERT_FLOAT(Real);
        
        const auto [ad,ade] = TwoProdFMA(A[0],A[3]);
        const auto [bc,bce] = TwoProdFMA(A[1],A[2]);

        const Real diff = ad  - bc;
        const Real e    = bce - ade;
            
        return (diff > e) - (diff < e);
    }
    
    template<typename Real>
    int Det2D_Corrected( cptr<Real> A )
    {
        ASSERT_FLOAT(Real);
        
        const auto [ad,ade] = TwoProdFMA(A[0],A[3]);
        const auto [bc,bce] = TwoProdFMA(A[1],A[2]);

        const Real diff = ad  - bc;
        const Real e    = bce - ade;
            
        return diff - e;
    }
    
    class RoundingModeBarrier
    {
    protected:

        int prev_state;
        int state;

        
    public:
        
        RoundingModeBarrier() = delete;
        
        explicit RoundingModeBarrier( int state_ )
        :   prev_state( std::fegetround() )
        ,   state ( state_ )
        {
            std::fesetround( state );
            
//            dump( prev_state );
//            
//            dump( state );
//            
//            dump( std::fegetround() );
        }
        
        ~RoundingModeBarrier()
        {
            std::fesetround( prev_state );
            
//            dump( prev_state );
//            
//            dump( state );
//            
//            dump( std::fegetround() );
        }
        
    }; // class RoundingModeBarrier
    
    
    template<typename Real_>
    class Interval;
    
    
    template<typename Real_>
    class Singleton
    {
        ASSERT_FLOAT(Real_)
        
    public:
        
        using Real = Real_;
        
        friend class Interval<Real>;
        
        using S_T = Singleton<Real>;
        using I_T = Interval<Real>;
        
    protected:
        
        Real x;
        
    public:
        
        Singleton()
        : x{ 0 }
        {}
        
        explicit Singleton( const Real & x_ )
        : x{ x_ }
        {}
        
        ~Singleton() = default;
        
        // Copy constructor
        Singleton( const Singleton & other )
        :   x { other.x }
        {}
        
        friend void swap (Singleton & s, Singleton & t ) noexcept
        {
            // see https://stackoverflow.com/questions/5695548/public-friend-swap-member-function for details
            using std::swap;
            
            swap( s.x, t.x );
        }
        
        // (Copy-)assignment operator
        Singleton & operator=( Singleton other )
        {
            swap( *this, other );
            
            return *this;
        }
        
        // Move constructor
        Singleton( Singleton && other ) noexcept
        {
            swap(*this, other);
        }
        
        Real Lower() const
        {
            return x;
        }
        
        Real Upper() const
        {
            return x;
        }
    
        
//######################################################
//##                  Univariate                      ##
//######################################################
        
    public:
        
        force_inline friend S_T operator+( const S_T s )
        {
            return S_T{s.x};
        }
        
        force_inline friend S_T operator-( const S_T s )
        {
            return S_T{-s.x};
        }
        
        force_inline friend S_T Abs( const S_T s )
        {
            return S_T{Abs(s.x)};
        }
        
        force_inline friend I_T Square( const S_T s )
        {
            s.RoundingModeWarning("Square");
            
            return I_T::Create( (- s.x) * s.x,  s.x * s.x);
        }

        force_inline friend S_T Ramp( const S_T s )
        {
            return S_T{Ramp(s.x)};
        }
        
        force_inline friend Real Sign( const S_T s )
        {
            constexpr Real zero = 0;
            
            return static_cast<Real>( (zero < s.x) - (s.x < zero) );
        }
        
        force_inline friend int IntSign( const S_T s )
        {
            constexpr Real zero = 0;
            
            return static_cast<int>(zero < s.x) - static_cast<int>(s.x < zero);
        }
        
        

        
//######################################################
//##                   Bivariate                      ##
//######################################################
    
    public:
        
        force_inline friend I_T operator+( const S_T s, const S_T t )
        {
            s.RoundingModeWarning("operator+ (SS)");
            
            return I_T::Create( (-s.x) + (-t.x), s.x + t.s );
        }
        
        
        force_inline friend I_T operator-( const S_T s, const S_T t )
        {
            s.RoundingModeWarning("operator- (SS)");
            
            return I_T::Create( (-s.x) + t.x , s.x + (-t.x) );
        }
        
        force_inline friend I_T operator*( const S_T s, const S_T t )
        {
            s.RoundingModeWarning("operator* (SS)");
            
            return I_T::Create( (-s.x) * t.x, s.x * t.x );
        }
        
        
        force_inline friend S_T Max( const S_T s, const S_T t )
        {
            return S_T( Max( s.x, t.x ) );
        }
        
        force_inline friend S_T Min( const S_T s, const S_T t )
        {
            return S_T( Min( s.x, t.x ) );
        }
        
        
//######################################################
//##                   Trivariate                     ##
//######################################################
        
        
        force_inline friend I_T fma( const S_T r, const S_T s, const S_T t )
        {
            r.RoundingModeWarning("fma (SSS)");
            
            return I_T::Create( std::fma( r.x, -s.x, -t.x ), std::fma( r.x, s.x, t.x ) );
        }
        
        
        void RoundingModeWarning( cref<std::string> tag ) const
        {
            #ifdef CHECK_ROUNDING_MODE
          
            if( std::fegetround() != FE_UPWARD )
            {
                eprint(ClassName()+"::" + tag + ": Rounding mode not set to FE_UPWARD.");
            }
            #endif
        }
        
        std::string ClassName() const
        {
            return std::string("Singleton<") + TypeName<Real> + ">";
        }
        
    }; // class Singleton
    
    
    
    
    
    template<typename Real_>
    class Interval
    {
        ASSERT_FLOAT(Real_)
        
    public:
        
        using Real = Real_;

        friend class Singleton<Real>;
        
        
        using R_T = Real;
        using I_T = Interval<Real>;
        using S_T = Singleton<Real>;
        
        
        
    protected:
        
        Real a;
        Real b;
        
    public:
        
        Interval()
        : a{ 0 }
        , b{ 0 }
        {}
        
        explicit Interval( cref<R_T> x )
        : a{ -x }
        , b{  x }
        {}
        
        explicit Interval( cref<S_T> s )
        : a{ -s.x }
        , b{  s.x }
        {}
        
        
        // Copy constructor
        Interval( cref<I_T> J )
        :   a { J.a }
        ,   b { J.b }
        {}
        
        explicit Interval( const R_T lower_, const R_T upper_ )
        : a{ -Min(lower_,upper_) }
        , b{  Max(lower_,upper_) }
        {}
        
        ~Interval() = default;
        
    
#if defined(LTEMPLATE_H) || defined(MATHEMATICA)
        
        explicit Interval( cref<mma::TensorRef<Real>> A )
        : Interval{ A.data()[0], A.data()[1]  }
        {}
        
        mma::TensorRef<Real> friend to_MTensorRef( const I_T I )
        {
            auto R = mma::makeVector<Real>( 2 );
            
            R.data()[0] = -I.a;
            R.data()[1] =  I.b;
            
            return R;
        }
#endif

        
        
        static Interval Create( const R_T a, const R_T b )
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
        
        friend void swap( I_T & I, I_T & J ) noexcept
        {
            // see https://stackoverflow.com/questions/5695548/public-friend-swap-member-function for details
            using std::swap;
            
            swap( I.a, J.a );
            swap( I.b, J.b );
        }
        
        // (Copy-)assignment operator
        Interval & operator=( I_T other )
        {
            swap( *this, other );
            
            return *this;
        }
        
        // Move constructor
        Interval( I_T && other ) noexcept
        {
            swap(*this, other);
        }
      
        
//######################################################
//##                    Access                        ##
//######################################################
    
    public:

        Real Upper() const
        {
            return b;
        }
        
        Real Lower() const
        {
            return -a;
        }
        
        Real Width() const
        {
            return (a + b);
        }
        
        Real EpsRelativeWidth() const
        {
            return ( (a + b) / Max(Abs(a),Abs(b)) ) / Scalar::eps<Real>;
        }
        
//######################################################
//##                   Booleans                       ##
//######################################################

    public:
        
        force_inline bool SingletonQ() const
        {
            return (-a==b);
        }
        
        force_inline bool ContainsZeroQ() const
        {
            return (b >= 0) && (a >= 0);
        }
        
        force_inline bool ContainsQ( const R_T x ) const
        {
            return (b >= x) && (-a <= x);
        }
        
        force_inline bool NotContainsQ( const R_T x ) const
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
        

        force_inline friend bool operator<( const I_T I, const I_T J )
        {
            return I.b < -J.a;
        }
        
        force_inline friend bool operator>( const I_T I, const I_T J )
        {
            return -I.a > J.b;
        }
        
        
        
        force_inline friend bool operator<( const I_T I, const R_T x )
        {
            return I.b < x;
        }
        
        force_inline friend bool operator<( const I_T I, const S_T s )
        {
            return I < s.x;
        }
        
        force_inline friend bool operator>( const R_T x, const I_T I )
        {
            return I < x;
        }
        
        force_inline friend bool operator>( const S_T s, const I_T I )
        {
            return I < s.x;
        }
        
        force_inline friend bool operator>( const I_T I, const R_T x )
        {
            return -I.a > x;
        }
        
        force_inline friend bool operator>( const I_T I, const S_T s )
        {
            return I > s.x;
        }
        
        force_inline friend bool operator<( const R_T x, const I_T I )
        {
            return I > x;
        }
        
        force_inline friend bool operator<( const S_T s, const I_T I )
        {
            return I > s.x;
        }
        
//######################################################
//##                  Univariate                      ##
//######################################################
        
    public:
        
        force_inline friend I_T operator+( const I_T I )
        {
            return Create( I.a, I.b );
        }
        
        force_inline friend I_T operator-( const I_T I )
        {
            return Create( I.b, I.a );
        }
        
        force_inline friend I_T Abs( const I_T I )
        {
            const Real A = Abs(I.a);
            const Real B = Abs(I.b);
            
            return Create( I.ContainsZeroQ() ? 0 : Min(A,B), Max(A,B) );
        }
        
        force_inline friend I_T Square( const I_T I )
        {
            I.RoundingModeWarning("Square");
            
            const Real A = I.a * I.a;
            const Real B = I.b * I.b;
            
            return Create( I.ContainsZeroQ() ? 0 : Min(A,B), Max(A,B) );
        }

        force_inline friend I_T Ramp( const I_T I )
        {
            return Create( Min(Scalar::Zero<Real>,I.a), Max(Scalar::Zero<Real>,I.b) );
        }
        
        
//        force_inline friend Real Sign( cref<Interval> I )
//        {
//            constexpr Real zero = Scalar::Zero<Real>;
//            constexpr Real one  = Scalar::One<Real>;
//            
//            return (I > zero) ? one : ((I < zero) ? -one : zero);
//        }
        
        force_inline friend Real Sign( const I_T I )
        {
            constexpr Real zero = 0;
            
            return static_cast<Real>( (zero < I) - (I < zero) );
        }
        
        force_inline friend int IntSign( const I_T I )
        {
            constexpr Real zero = 0;
            
            return static_cast<int>(zero < I) - static_cast<int>(I < zero);
        }
        
        

        
//######################################################
//##                   Bivariate                      ##
//######################################################
    
    public:
        
        force_inline friend I_T operator+( const I_T I, const I_T J )
        {
            I.RoundingModeWarning("operator+ (II)");
            
            return Create( I.a + J.a, I.b + J.b );
        }
        
        force_inline friend I_T operator+( const I_T I, const R_T x )
        {
            I.RoundingModeWarning("operator+ (IR)");
            
            return Create( I.a - x, I.b + x );
        }
        
        force_inline friend I_T operator+( const I_T I, const S_T s )
        {
            return I + s.x;
        }
        
        
        force_inline friend I_T operator+( const R_T x, const I_T I )
        {
            return I + x;
        }
        
        force_inline friend I_T operator+( const S_T s, const I_T I )
        {
            return I + s.x;
        }
        
        
        
        force_inline friend I_T operator-( const I_T I, const I_T J )
        {
            I.RoundingModeWarning("operator- (II)");
            
            return Create( I.a + J.b , I.b + J.a );
        }
        
        force_inline friend I_T operator-( const I_T I, const R_T x )
        {
            I.RoundingModeWarning("operator- (IR)");
            
            return Create( I.a + x , I.b - x );
        }

        force_inline friend I_T operator-( const I_T I, const S_T s )
        {
            return I + s.x;
        }
        

        force_inline friend I_T operator-( const R_T x, const I_T I )
        {
            I.RoundingModeWarning("operator- (RI)");
            
            return Create( -x + I.b , x + I.a );
        }

        force_inline friend I_T operator-( const S_T s, const I_T J )
        {
            return s.x - J;
        }

        
        force_inline friend I_T mult_II_pos_pos( const I_T I, const I_T J )
        {
            // I.Lower() > 0, I.Upper() > 0, J.Lower() > 0, J.Upper() > 0
            // Then solution is
            // [ I.Lower() * J.Lower(), I.Upper() * J.Upper() ]

//            return Create( (-I.Lower()) * J.Lower(), I.Upper() * J.Upper() );
            
            return Create( I.a * (-J.a), I.b * J.b );
        }
        
        force_inline friend I_T mult_II_branching( const I_T I, const I_T J )
        {
            constexpr Real zero = 0;
            
            
            if( I > zero )
            {
                if( J > zero )
                {
                    return mult_II_pos_pos( I, J );
                }
                else if( J < zero )
                {
                    // I.Lower() > 0, I.Upper() > 0, J.Lower() < 0, J.Upper() < 0
                    // Then solution is
                    // [ I.Upper() * J.Lower(), I.Lower() * J.Upper() ]

//                    return Create( I.Upper() * (-J.Lower()), I.Lower() * J.Upper() );

                    return Create( I.b * J.a, (-I.a) * J.b );
                }
                else
                {
                    // I.Lower() > 0, I.Upper() > 0, J.Lower() <= 0, J.Upper() >= 0
                    // Then solution is
                    // [ I.Upper() * J.Lower(), I.Upper() * J.Upper() ]

//                    return Create( I.Upper() * (-J.Lower()), I.Upper() * J.Upper() );

                    return Create( I.b * J.a, I.b * J.b );
                }
            }
            else if( I < zero )
            {
                if( J > zero )
                {
                    // I.Lower() < 0, I.Upper() < 0, J.Lower() > 0, J.Upper() > 0
                    // Then solution is
                    // [ I.Lower() * J.Upper(), I.Upper() * J.Lower() ]

//                    return Create( (-I.Lower()) * J.Upper(), I.Upper() * J.Lower() );

                    return Create( I.a * J.b, I.b * (-J.a) );
                }
                else if( J < zero )
                {
                    // I.Lower() < 0, I.Upper() < 0, J.Lower() < 0, J.Upper() < 0
                    // Then solution is
                    // [ I.Upper() * J.Upper(), I.Lower() * J.Lower() ]

//                    return Create( (-I.Upper()) * J.Upper(), I.Lower() * J.Lower() );

                    return Create( (-I.b) * J.b, I.a * J.a );
                }
                else
                {
                    // I.Lower() < 0, I.Upper() < 0, J.Lower() <= 0, J.Upper() >= 0
                    // Then solution is
                    // [ I.Lower() * J.Upper(), I.Lower() * J.Lower() ]

//                    return Create( (-I.Lower() * J.Upper(), I.Lower() * J.Lower() );

                    return Create( I.a * J.b, I.a * J.a );
                }
            }
            else
            {
                if( J > zero )
                {
                    // I.Lower() <= 0, I.Upper() >= 0, J.Lower() > 0, J.Upper() > 0
                    // Then solution is
                    // [ I.Lower() * J.Upper(), I.Upper() * J.Upper() ]

//                    return Create( (-I.Lower()) * J.Upper(), I.Upper() * J.Upper() );

                    return Create( I.a * J.b, I.b * J.b );
                }
                else if( J < zero )
                {
                    // I.Lower() <= 0, I.Upper() >= 0, J.Lower() < 0, J.Upper() < 0
                    // Then solution is
                    // [ I.Upper() * J.Lower(), I.Lower() * J.Lower() ]

//                    return Create( I.Upper() * (-J.Lower()), I.Lower() * J.Lower() );

                    return Create( I.b * J.a, I.a * J.a );
                }
                else
                {
                    // I.Lower() <= 0, I.Upper() => 0, J.Lower() <= 0, J.Upper() => 0
                    // Then solution is
                    // [ Min(I.Lower() * J.Upper(), I.Upper() * J.Lower(),
                    //   Max(I.Lower() * J.Lower(), I.Upper() * J.Upper() ]

                    return Create( Max( I.a * J.b, I.b * J.a ), Max( I.a * J.a, I.b * J.b ) );
                }
            }
        }
        
        force_inline friend I_T mult_II_switch( const I_T I, const I_T J )
        {
            switch( IntSign(I) )
            {
                case 1:
                {
                    switch( IntSign(J) )
                    {
                        case 1:
                        {
                            return mult_II_pos_pos( I, J );
                        }
                        case -1:
                        {
                            // I.Lower() > 0, I.Upper() > 0, J.Lower() < 0, J.Upper() < 0
                            // Then solution is
                            // [ I.Upper() * J.Lower(), I.Lower() * J.Upper() ]
                            
                            // return Create( I.Upper() * (-J.Lower()), I.Lower() * J.Upper() );
                            
                            return Create( I.b * J.a, (-I.a) * J.b );
                        }
                        case 0:
                        {
                            // I.Lower() > 0, I.Upper() > 0, J.Lower() <= 0, J.Upper() >= 0
                            // Then solution is
                            // [ I.Upper() * J.Lower(), I.Upper() * J.Upper() ]
                            
                            // return Create( I.Upper() * (-J.Lower()), I.Upper() * J.Upper() );
                            
                            return Create( I.b * J.a, I.b * J.b );
                        }
                    }
                }
                case -1:
                {
                    switch( IntSign(J) )
                    {
                        case 1:
                        {
                            // I.Lower() < 0, I.Upper() < 0, J.Lower() > 0, J.Upper() > 0
                            // Then solution is
                            // [ I.Lower() * J.Upper(), I.Upper() * J.Lower() ]
                            
                            // return Create( (-I.Lower()) * J.Upper(), I.Upper() * J.Lower() );
                            
                            return Create( I.a * J.b, I.b * (-J.a) );
                        }
                        case -1:
                        {
                            // I.Lower() < 0, I.Upper() < 0, J.Lower() < 0, J.Upper() < 0
                            // Then solution is
                            // [ I.Upper() * J.Upper(), I.Lower() * J.Lower() ]
                            
                            // return Create( (-I.Upper()) * J.Upper(), I.Lower() * J.Lower() );
                            
                            return Create( (-I.b) * J.b, I.a * J.a );
                        }
                        case 0:
                        {
                            // I.Lower() < 0, I.Upper() < 0, J.Lower() <= 0, J.Upper() >= 0
                            // Then solution is
                            // [ I.Lower() * J.Upper(), I.Lower() * J.Lower() ]
                            
                            // return Create( (-I.Lower() * J.Upper(), I.Lower() * J.Lower() );
                            
                            return Create( I.a * J.b, I.a * J.a );
                        }
                    }
                }
                case 0:
                {
                    switch( IntSign(J) )
                    {
                        case 1:
                        {
                            // I.Lower() <= 0, I.Upper() >= 0, J.Lower() > 0, J.Upper() > 0
                            // Then solution is
                            // [ I.Lower() * J.Upper(), I.Upper() * J.Upper() ]
                            
                            // return Create( (-I.Lower()) * J.Upper(), I.Upper() * J.Upper() );
                            
                            return Create( I.a * J.b, I.b * J.b );
                        }
                        case -1:
                        {
                            // I.Lower() <= 0, I.Upper() >= 0, J.Lower() < 0, J.Upper() < 0
                            // Then solution is
                            // [ I.Upper() * J.Lower(), I.Lower() * J.Lower() ]
                            
                            // return Create( I.Upper() * (-J.Lower()), I.Lower() * J.Lower() );
                            
                            return Create( I.b * J.a, I.a * J.a );
                        }
                        case 0:
                        {
                            // I.Lower() <= 0, I.Upper() => 0, J.Lower() <= 0, J.Upper() => 0
                            // Then solution is
                            // [ Min(I.Lower() * J.Upper(), I.Upper() * J.Lower(),
                            //   Max(I.Lower() * J.Lower(), I.Upper() * J.Upper() ]
                            
                            return Create( Max( I.a * J.b, I.b * J.a ), Max( I.a * J.a, I.b * J.b ) );
                        }
                    }
                }
                    
            } // switch( IntSign(I) )
            
            return I_T();
        }
        
        
        force_inline friend I_T mul_II_bruteforce( const I_T I, const I_T J )
        {
            const Real A = Max(
                Max( (-I.a) * ( J.a), ( I.a) * ( J.b) ),
                Max( ( I.b) * ( J.a), (-I.b) * ( J.b) )
            );
            
            const Real B = Max(
                Max( ( I.a) * ( J.a), (-I.a) * ( J.b) ),
                Max( (-I.b) * ( J.a), ( I.b) * ( J.b) )
            );
            
            return Create( A, B );
        }
        
        force_inline friend I_T operator*( const I_T I, const I_T J )
        {
            I.RoundingModeWarning("operator* (II)");
            
            return mul_II_bruteforce( I, J );
            
//            return mult_II_branching( I, J );
            
//            return mult_II_switch( I, J );
        }
        
        force_inline friend I_T operator*( const I_T I, const R_T x )
        {
            I.RoundingModeWarning("operator* (IR)");
            
            return Create(
                Max( ( I.a) * x, (-I.b) * x ),
                Max( (-I.a) * x, ( I.b) * x )
            );
        }
        
        force_inline friend I_T operator*( const I_T I, const S_T s )
        {
            return I * s.x;
        }
        
        
        force_inline friend I_T operator*( const R_T x, const I_T I )
        {
            return I * x;
        }
        
        force_inline friend I_T operator*( const S_T s, const I_T I )
        {
            return I * s.x;
        }
        
        
        
        
        
        force_inline friend I_T Max( const I_T I, const I_T J )
        {
            return Create( Min(I.a,J.a), Max(I.b,J.b) );
        }
        

        force_inline friend I_T Max( const I_T I, const R_T x )
        {
            return Create( Min(I.a,-x), Max(I.b,x) );
        }
        
        force_inline friend I_T Max( const I_T I, const S_T s )
        {
            return Max(I,s.x);
        }
        
        
        force_inline friend I_T Max( const R_T x, const I_T I )
        {
            return Create( Min(-x,I.a), Max(x,I.b) );
        }
        
        force_inline friend I_T Max( const S_T s, const I_T I )
        {
            return Max(I,s.x);
        }
        
        
        force_inline friend I_T Min( const I_T I, const I_T J )
        {
            return Create( Max(I.a,J.a), Min(I.b,J.b) );
        }
        
        
        force_inline friend I_T Min( const I_T I, const R_T x )
        {
            return Create( Max(I.a,-x), Min(I.b,x) );
        }
        
        force_inline friend I_T Min( const I_T I, const S_T s )
        {
            return Min(I,s.x);
        }

        force_inline friend I_T Min( const R_T x, const I_T I )
        {
            return Min(I,x);
        }
        
        force_inline friend I_T Min( const S_T s, const I_T I )
        {
            return Min(I,s.x);
        }
        
//######################################################
//##                   Trivariate                      ##
//######################################################
        
        force_inline friend I_T fma( const I_T I, const I_T J, const I_T K )
        {
            I.RoundingModeWarning("fma (III)");

            const Real A = Max(
                Max( std::fma( -I.a, J.a, K.a ), std::fma(  I.a, J.b, K.a) ),
                Max( std::fma(  I.b, J.a, K.a ), std::fma( -I.b, J.b, K.a) )
            );
            
            const Real B = Max(
                Max( std::fma(  I.a, J.a, K.b ), std::fma( -I.a, J.b, K.b ) ),
                Max( std::fma( -I.b, J.a, K.b ), std::fma(  I.b, J.b, K.b ) )
            );
            
            return Create(A,B);
        }
        
        force_inline friend I_T fma( const I_T I, const I_T J, const R_T z )
        {
            I.RoundingModeWarning("fma (IIR)");

            const Real A = Max(
                Max( std::fma( -I.a, J.a, -z ), std::fma(  I.a, J.b, -z) ),
                Max( std::fma(  I.b, J.a, -z ), std::fma( -I.b, J.b, -z) )
            );
            
            const Real B = Max(
                Max( std::fma(  I.a, J.a, z ), std::fma( -I.a, J.b, z ) ),
                Max( std::fma( -I.b, J.a, z ), std::fma(  I.b, J.b, z ) )
            );
            
            return Create(A,B);
        }
        
        force_inline friend I_T fma( const I_T I, const I_T J, const S_T t )
        {
            return fma(I,J,t.x);
        }

        
        force_inline friend I_T fma( const R_T x, const I_T J, const I_T K )
        {
            J.RoundingModeWarning("fma (RII)");

            const Real A = Max( std::fma(  x, J.a, K.a ), std::fma( -x, J.b, K.a ) );
            const Real B = Max( std::fma( -x, J.a, K.b ), std::fma(  x, J.b, K.b ) );
            
            return Create(A,B);
        }
        
        force_inline friend I_T fma( const S_T s, const I_T J, const I_T K )
        {
            return fma( s.x, J, K );
        }
        
        force_inline friend I_T fma( const I_T J, const R_T x, const I_T K )
        {
            return fma( x, J, K );
        }
        
        force_inline friend I_T fma( const I_T J, const S_T s, const I_T K )
        {
            return fma( s.x, J, K );
        }
        
        
        force_inline friend I_T fma( const R_T x, const R_T y, const I_T K )
        {
            K.RoundingModeWarning("fma (RRI)");
            
            return Create(std::fma( -x, y, K.a ), std::fma( x, y, K.b ) );
        }
        
        force_inline friend I_T fma( const S_T s, const S_T t, const I_T K )
        {
            return fma( s.x, t.x, K );
        }
        
        
        force_inline friend I_T fma( const R_T x, const I_T I, const R_T z )
        {
            I.RoundingModeWarning("fma (RIR)");
            
            return Create (
                Max( std::fma( x,  I.a, -z ), std::fma( x, -I.b, -z ) ),
                Max( std::fma( x, -I.a,  z ), std::fma( x,  I.b,  z ) )
            );
        }
        
        force_inline friend I_T fma( const S_T r, const I_T I, const S_T t )
        {
            return fma( r.x, I, t.x );
        }
        
        
        force_inline friend I_T fma( const I_T I, const R_T x, const R_T z )
        {
            return fma( x, I, z );
        }
        
        force_inline friend I_T fma( const I_T I, const S_T r, const S_T t )
        {
            return fma( r.x, I, t.x );
        }
        


        

        
        
        
        void RoundingModeWarning( cref<std::string> tag ) const
        {
            #ifdef CHECK_ROUNDING_MODE
          
            if( std::fegetround() != FE_UPWARD )
            {
                eprint(ClassName()+"::" + tag + ": Rounding mode not set to FE_UPWARD.");
            }
            #endif
        }
        
        
        
        std::string ClassName() const
        {
            return std::string("Interval<") + TypeName<Real> + ">";
        }
        
        
    }; // class Interval
    

    template<typename Real>
    std::string ToString( const Interval<Real> I )
    {
        return std::string("[ ") + Tools::ToString(I.Lower()) + ", " + Tools::ToString(I.Upper()) + " ]";
    }
    
    
    
    template<int D, bool fmaQ = true, typename c_T, typename x_T>
    decltype( c_T(1) * x_T(1) ) EvaluatePolynomial_Horner( cptr<c_T> c, const x_T x )
    {
        using std::fma;
        
        using oux_T = decltype( c_T(1) * x_T(1) );
        
        if( D <= 0 )
        {
            return oux_T{c[0]};
        }
        
        if constexpr ( fmaQ )
        {
            oux_T r = fma( c[D], x, c[D-1] );
            
            for( int i = D-1; i --> 0;  )
            {
                
                r = fma( r, x, c[i] );
            }
            
            return r;
        }
        else
        {
            oux_T r = c[D] * x + c[D-1];
            
            for( int i = D-1; i --> 0;  )
            {
                
                r = r * x + c[i];
            }
            
            return r;
        }
        
    }
    
} // namespace Tools



#include "Oriented2D.hpp"
#include "Oriented3D.hpp"
