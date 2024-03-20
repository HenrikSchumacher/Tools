#pragma once

namespace Tools
{
    
    template<typename Real, typename UInt>
    class uniform_dist
    {
        ASSERT_REAL(Real);
        
    private:
        
        const Real a     = 0;
        const Real b     = 1;
        
        const Real scale = 1;
                
    public:
        
        uniform_dist( const Real a_, const Real b_ ) noexcept
        : a(a_)
        , b(b_)
        , scale( (b-a) / std::numeric_limits<UInt>::max() )
        {}
        
        ~uniform_dist() = default;
        
        template<typename R>
        force_inline Real operator()( R & r ) const
        {
            return a + static_cast<Real>(r()) * scale;
        }
    };
    
    template<typename Real, typename UInt>
    class fake_gaussian_dist
    {
        ASSERT_REAL(Real);
        
        static constexpr Real scale = Scalar::One<Real> / (1ull << 20);
        
    public:
        
        fake_gaussian_dist() = default;
        
        ~fake_gaussian_dist() = default;
        
        template<typename PRNG_T>
        force_inline Real operator()( PRNG_T & random_engine ) const
        {
            const UInt r = random_engine();
            
            return ((r & 0x1fffff) + ((r >> 21) & 0x1fffff) + ((r >> 42) & 0x1fffff)) * scale - Scalar::Three<Real>;
        }
    };
    
    
    template<typename Real, typename UInt>
    class BoxMuller
    {
    public:
        
        ASSERT_REAL(Real);
        
    private:
        
        Real X;
        Real Y;
        
        uniform_dist<Real,UInt64> r_dist { Scalar::One<Real>, Scalar::Zero<Real> };
        uniform_dist<Real,UInt64> phi_dist { -Scalar::Pi<Real>, Scalar::Pi<Real> };
        
        bool computedQ = 0;
        
        
    public:
        
        BoxMuller() = default;
        
        ~BoxMuller() = default;
                
        template<typename PRNG_T>
        force_inline void operator()( PRNG_T & random_engine, Real & x, Real & y )
        {
            const Real r   = Sqrt(-Scalar::Two<Real> * std::log( r_dist(random_engine)) );
            
            const Real phi = phi_dist(random_engine);
            
            x = r * std::cos(phi);
            y = r * std::sin(phi);
        }
        
        template<typename PRNG_T>
        force_inline Real operator()( PRNG_T & random_engine )
        {
            if( computedQ )
            {
                computedQ = false;
                return Y;
            }
            else
            {
                const Real r   = Sqrt( -Scalar::Two<Real> * std::log( r_dist(random_engine)) );
                
                const Real phi = phi_dist(random_engine);
                
                X = r * std::cos(phi);
                Y = r * std::sin(phi);
                
                computedQ = true;
                
                return X;
            }
        }
        
    };
    
    template<typename Real, typename UInt>
    class Gaussian
    {
        
    public:
        
        ASSERT_REAL(Real);
        
    private:
        
        Real X;
        Real Y;
        Real r2;
        
        uniform_dist<Real,UInt64> dist { -Scalar::One<Real>, Scalar::One<Real> };
        
        bool computedQ = 0;
        
    public:
        
        Gaussian() = default;
        
        ~Gaussian() = default;
        
        template<typename PRNG_T>
        force_inline Real operator()( PRNG_T & random_engine )
        {
            if( computedQ )
            {
                computedQ = false;
                return Y;
            }
            else
            {
                X = dist(random_engine);
                Y = dist(random_engine);
                
                r2 = X * X + Y * Y;
                
                [[unlikely]] while ( r2 > Scalar::One<Real> || r2 == Scalar::Zero<Real> )
                {
                    X = dist(random_engine);
                    Y = dist(random_engine);
                    
                    r2 = X * X + Y * Y;
                }
                
                const Real r  = Sqrt( - Scalar::Two<Real> * std::log( r2 ) / r2 );
                
                X *= r;
                Y *= r;
                
                computedQ = true;
                
                return X;
            }
        }
        
    };

}

 


