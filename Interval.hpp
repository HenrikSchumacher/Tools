#pragma once

// Beware: At compile time all floating point rounding is towards nearest!!!

#include <cfenv>

#include "Tools.hpp"

// TODO: Read this:
// https://www.codeproject.com/Articles/1040839/Interval-arithmetic-using-round-to-nearest-mode-pa



// TODO: integer power.
// TODO: operator/

// TODO: Things from the Scalar namespace, e.g., RealQ, ComplexQ, Prec etc.
// TODO: constants! (e.g. Pi)


namespace Tools
{
    class RoundingModeBarrier
    {
    protected:

        int prev_state;
        int state;

        
    public:
        
        explicit RoundingModeBarrier( int state_ )
        :   prev_state( std::fegetround() )
        ,   state ( state_ )
        {
            std::fesetround( state_ );
        }
        
        ~RoundingModeBarrier()
        {
            std::fesetround( prev_state );
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
        
        force_inline friend S_T operator+( cref<S_T> s )
        {
            return S_T{s.x};
        }
        
        force_inline friend S_T operator-( cref<S_T> s )
        {
            return S_T{-s.x};
        }
        
        force_inline friend S_T Abs( cref<S_T> s )
        {
            return S_T{Abs(s.x)};
        }
        
        force_inline friend I_T Square( cref<S_T> s )
        {
            s.RoundingModeWarning("Square");
            
            return I_T::Create( (- s.x) * s.x,  s.x * s.x);
        }

        force_inline friend S_T Ramp( cref<S_T> s )
        {
            return S_T{Ramp(s.x)};
        }
        
        force_inline friend Real Sign( cref<S_T> s )
        {
            constexpr Real zero = 0;
            
            return static_cast<Real>( (zero < s.x) - (s.x < zero) );
        }
        
        force_inline friend int IntSign( cref<S_T> s )
        {
            constexpr Real zero = 0;
            
            return static_cast<int>(zero < s.x) - static_cast<int>(s.x < zero);
        }
        
        

        
//######################################################
//##                   Bivariate                      ##
//######################################################
    
    public:
        
        force_inline friend I_T operator+( cref<S_T> s, cref<S_T> t )
        {
            s.RoundingModeWarning("operator+ (SS)");
            
            return I_T::Create( (-s.x) + (-t.x), s.x + t.s );
        }
        
        
        force_inline friend I_T operator-( cref<S_T> s, cref<S_T> t )
        {
            s.RoundingModeWarning("operator- (SS)");
            
            return I_T::Create( (-s.x) + t.x , s.x + (-t.x) );
        }
        
        force_inline friend I_T operator*( cref<S_T> s, cref<S_T> t )
        {
            s.RoundingModeWarning("operator* (SS)");
            
            return I_T::Create( (-s.x) * t.x, s.x * t.x );
        }
        
        
        force_inline friend S_T Max( cref<S_T> s, cref<S_T> t )
        {
            return S_T( Max( s.x, t.x ) );
        }
        
        force_inline friend S_T Min( cref<S_T> s, cref<S_T> t )
        {
            return S_T( Min( s.x, t.x ) );
        }
        
        
//######################################################
//##                   Trivariate                     ##
//######################################################
        
        
        force_inline friend I_T fma( cref<S_T> r, cref<S_T> s, cref<S_T> t )
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
        
        mma::TensorRef<Real> friend to_MTensorRef( cref<I_T> I )
        {
            auto R = mma::makeVector<Real>( 2 );
            
            R.data()[0] = -I.a;
            R.data()[1] =  I.b;
            
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
        
        force_inline bool ContainsQ( cref<R_T> x ) const
        {
            return (b >= x) && (-a <= x);
        }
        
        force_inline bool NotContainsQ( cref<R_T> x ) const
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
        

        force_inline friend bool operator<( cref<I_T> I, cref<I_T> J )
        {
            return I.b < -J.a;
        }
        
        force_inline friend bool operator>( cref<I_T> I, cref<I_T> J )
        {
            return -I.a > J.b;
        }
        
        
        
        force_inline friend bool operator<( cref<I_T> I, cref<R_T> x )
        {
            return I.b < x;
        }
        
        force_inline friend bool operator<( cref<I_T> I, cref<S_T> s )
        {
            return I < s.x;
        }
        
        force_inline friend bool operator>( cref<R_T> x, cref<I_T> I )
        {
            return I < x;
        }
        
        force_inline friend bool operator>( cref<S_T> s, cref<I_T> I )
        {
            return I < s.x;
        }
        
        force_inline friend bool operator>( cref<I_T> I, cref<R_T> x )
        {
            return -I.a > x;
        }
        
        force_inline friend bool operator>( cref<I_T> I, cref<S_T> s )
        {
            return I > s.x;
        }
        
        force_inline friend bool operator<( cref<R_T> x, cref<I_T> I )
        {
            return I > x;
        }
        
        force_inline friend bool operator<( cref<S_T> s, cref<I_T> I )
        {
            return I > s.x;
        }
        
//######################################################
//##                  Univariate                      ##
//######################################################
        
    public:
        
        force_inline friend I_T operator+( cref<I_T> I )
        {
            return Create( I.a, I.b );
        }
        
        force_inline friend I_T operator-( cref<I_T> I )
        {
            return Create( I.b, I.a );
        }
        
        force_inline friend I_T Abs( cref<I_T> I )
        {
            const Real A = Abs(I.a);
            const Real B = Abs(I.b);
            
            return Create( I.ContainsZeroQ() ? 0 : Min(A,B), Max(A,B) );
        }
        
        force_inline friend I_T Square( cref<I_T> I )
        {
            I.RoundingModeWarning("Square");
            
            const Real A = I.a * I.a;
            const Real B = I.b * I.b;
            
            return Create( I.ContainsZeroQ() ? 0 : Min(A,B), Max(A,B) );
        }

        force_inline friend I_T Ramp( cref<I_T> I )
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
        
        force_inline friend Real Sign( cref<I_T> I )
        {
            constexpr Real zero = 0;
            
            return static_cast<Real>( (zero < I) - (I < zero) );
        }
        
        force_inline friend int IntSign( cref<I_T> I )
        {
            constexpr Real zero = 0;
            
            return static_cast<int>(zero < I) - static_cast<int>(I < zero);
        }
        
        

        
//######################################################
//##                   Bivariate                      ##
//######################################################
    
    public:
        
        force_inline friend I_T operator+( cref<I_T> I, cref<I_T> J )
        {
            I.RoundingModeWarning("operator+ (II)");
            
            return Create( I.a + J.a, I.b + J.b );
        }
        
        force_inline friend I_T operator+( cref<I_T> I, cref<R_T> x )
        {
            I.RoundingModeWarning("operator+ (IR)");
            
            return Create( I.a - x, I.b + x );
        }
        
        force_inline friend I_T operator+( cref<I_T> I, cref<S_T> s )
        {
            return I + s.x;
        }
        
        
        force_inline friend I_T operator+( cref<R_T> x, cref<I_T> I )
        {
            return I + x;
        }
        
        force_inline friend I_T operator+( cref<S_T> s, cref<I_T> I )
        {
            return I + s.x;
        }
        
        
        
        force_inline friend I_T operator-( cref<I_T> I, cref<I_T> J )
        {
            I.RoundingModeWarning("operator- (II)");
            
            return Create( I.a + J.b , I.b + J.a );
        }
        
        force_inline friend I_T operator-( cref<I_T> I, cref<R_T> x )
        {
            I.RoundingModeWarning("operator- (IR)");
            
            return Create( I.a + x , I.b - x );
        }

        force_inline friend I_T operator-( cref<I_T> I, cref<S_T> s )
        {
            return I + s.x;
        }
        

        force_inline friend I_T operator-( cref<R_T> x, cref<I_T> I )
        {
            I.RoundingModeWarning("operator- (RI)");
            
            return Create( -x + I.b , x + I.a );
        }

        force_inline friend I_T operator-( cref<S_T> s, cref<I_T> J )
        {
            return s.x - J;
        }

        
        force_inline friend I_T mult_II_pos_pos( cref<I_T> I, cref<I_T> J )
        {
            // I.Lower() > 0, I.Upper() > 0, J.Lower() > 0, J.Upper() > 0
            // Then solution is
            // [ I.Lower() * J.Lower(), I.Upper() * J.Upper() ]

//            return Create( (-I.Lower()) * J.Lower(), I.Upper() * J.Upper() );
            
            return Create( I.a * (-J.a), I.b * J.b );
        }
        
        force_inline friend I_T mult_II_branching( cref<I_T> I, cref<I_T> J )
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
        
        force_inline friend I_T mult_II_switch( cref<I_T> I, cref<I_T> J )
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
        
        
        force_inline friend I_T mul_II_bruteforce( cref<I_T> I, cref<I_T> J )
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
        
        force_inline friend I_T operator*( cref<I_T> I, cref<I_T> J )
        {
            I.RoundingModeWarning("operator* (II)");
            
            return mul_II_bruteforce( I, J );
            
//            return mult_II_branching( I, J );
            
//            return mult_II_switch( I, J );
        }
        
        force_inline friend I_T operator*( cref<I_T> I, cref<R_T> x )
        {
            I.RoundingModeWarning("operator* (IR)");
            
            return Create(
                Max( ( I.a) * x, (-I.b) * x ),
                Max( (-I.a) * x, ( I.b) * x )
            );
        }
        
        force_inline friend I_T operator*( cref<I_T> I, cref<S_T> s )
        {
            return I * s.x;
        }
        
        
        force_inline friend I_T operator*( cref<R_T> x, cref<I_T> I )
        {
            return I * x;
        }
        
        force_inline friend I_T operator*( cref<S_T> s, cref<I_T> I )
        {
            return I * s.x;
        }
        
        
        
        
        
        force_inline friend I_T Max( cref<I_T> I, cref<I_T> J )
        {
            return Create( Min(I.a,J.a), Max(I.b,J.b) );
        }
        

        force_inline friend I_T Max( cref<I_T> I, cref<R_T> x )
        {
            return Create( Min(I.a,-x), Max(I.b,x) );
        }
        
        force_inline friend I_T Max( cref<I_T> I, cref<S_T> s )
        {
            return Max(I,s.x);
        }
        
        
        force_inline friend I_T Max( cref<R_T> x, cref<I_T> I )
        {
            return Create( Min(-x,I.a), Max(x,I.b) );
        }
        
        force_inline friend I_T Max( cref<S_T> s, cref<I_T> I )
        {
            return Max(I,s.x);
        }
        
        
        force_inline friend I_T Min( cref<I_T> I, cref<I_T> J )
        {
            return Create( Max(I.a,J.a), Min(I.b,J.b) );
        }
        
        
        force_inline friend I_T Min( cref<I_T> I, cref<R_T> x )
        {
            return Create( Max(I.a,-x), Min(I.b,x) );
        }
        
        force_inline friend I_T Min( cref<I_T> I, cref<S_T> s )
        {
            return Min(I,s.x);
        }

        force_inline friend I_T Min( cref<R_T> x, cref<I_T> I )
        {
            return Min(I,x);
        }
        
        force_inline friend I_T Min( cref<S_T> s, cref<I_T> I )
        {
            return Min(I,s.x);
        }
        
//######################################################
//##                   Trivariate                      ##
//######################################################
        
        force_inline friend I_T fma( cref<I_T> I, cref<I_T> J, cref<I_T> K )
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
        
        force_inline friend I_T fma( cref<I_T> I, cref<I_T> J, cref<R_T> z )
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
        
        force_inline friend I_T fma( cref<I_T> I, cref<I_T> J, cref<S_T> t )
        {
            return fma(I,J,t.x);
        }

        
        force_inline friend I_T fma( cref<R_T> x, cref<I_T> J, cref<I_T> K )
        {
            J.RoundingModeWarning("fma (RII)");

            const Real A = Max( std::fma(  x, J.a, K.a ), std::fma( -x, J.b, K.a ) );
            const Real B = Max( std::fma( -x, J.a, K.b ), std::fma(  x, J.b, K.b ) );
            
            return Create(A,B);
        }
        
        force_inline friend I_T fma( cref<S_T> s, cref<I_T> J, cref<I_T> K )
        {
            return fma( s.x, J, K );
        }
        
        force_inline friend I_T fma( cref<I_T> J, cref<R_T> x, cref<I_T> K )
        {
            return fma( x, J, K );
        }
        
        force_inline friend I_T fma( cref<I_T> J, cref<S_T> s, cref<I_T> K )
        {
            return fma( s.x, J, K );
        }
        
        
        force_inline friend I_T fma( cref<R_T> x, cref<R_T> y, cref<I_T> K )
        {
            K.RoundingModeWarning("fma (RRI)");
            
            return Create(std::fma( -x, y, K.a ), std::fma( x, y, K.b ) );
        }
        
        force_inline friend I_T fma( cref<S_T> s, cref<S_T> t, cref<I_T> K )
        {
            return fma( s.x, t.x, K );
        }
        
        
        force_inline friend I_T fma( cref<R_T> x, cref<I_T> I, cref<R_T> z )
        {
            I.RoundingModeWarning("fma (RIR)");
            
            return Create (
                Max( std::fma( x,  I.a, -z ), std::fma( x, -I.b, -z ) ),
                Max( std::fma( x, -I.a,  z ), std::fma( x,  I.b,  z ) )
            );
        }
        
        force_inline friend I_T fma( cref<S_T> r, cref<I_T> I, cref<S_T> t )
        {
            return fma( r.x, I, t.x );
        }
        
        
        force_inline friend I_T fma( cref<I_T> I, cref<R_T> x, cref<R_T> z )
        {
            return fma( x, I, z );
        }
        
        force_inline friend I_T fma( cref<I_T> I, cref<S_T> r, cref<S_T> t )
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
    

    template<typename T>
    std::string ToString( const Interval<T> & I )
    {
        return std::string("[ ") + Tools::ToString(I.Lower()) + ", " + Tools::ToString(I.Upper()) + " ]";
    }
    
} // namespace Tools



#include "Oriented2D.hpp"
#include "Oriented3D.hpp"
