#pragma once

namespace Tools
{
    template<typename Scal_, typename Int_>
    class ProductAccumulator
    {
        using Scal    = Scal_;
        using Int     = Int_;
        using Real    = Scalar::Real<Scal>;
        using Complex = Scalar::Complex<Real>;
        
        static_assert( Scalar::RealQ<Scal> || Scalar::ComplexQ<Scal>, "" );
        
    private:
        
        Real x = 0.5; // mantissa of real part
        Real y = 0;   // mantissa of imag part
        
//        Int e_x = 1; // mantissa of real part
//        Int e_y = 0; // mantissa of imag part
        
        Int  e = 1; // common exponent
        
    public:
        
        ProductAccumulator() = default;
        
        ProductAccumulator( const Scal init )
        {
            if constexpr ( Scalar::RealQ<Scal> )
            {
                std::tie(x,e)   = mantissa_exponent( init );
            }
            else
            {
                std::tie(x,y,e) = mantissa_exponent( init );
            }
        }
        
        ~ProductAccumulator() = default;
        
        
    private:
        
        std::tuple<Real,Int> mantissa_exponent( const Real z )
        {
            int exponent;
            Real mantissa = std::frexp( z, &exponent );
            
            return std::tuple<Real,Int>(mantissa,exponent);
        }
        
        std::tuple<Real,Real,Int> mantissa_exponent( const Complex z )
        {
            auto [u,e_u] = mantissa_exponent( Re(z) );
            auto [v,e_v] = mantissa_exponent( Im(z) );
            
            Int d = e_u - e_v;
            
            Int exponent;
            
            if( d >= 0 )
            {
                exponent = e_u;
                v = v * Power(Real(2),-d);
            }
            else
            {
                exponent = e_v;
                u = u * Power(Real(2),d);
            }
            
            return std::tuple<Real,Real,Int>(u,v,exponent);
        }
        
    public:
        
        ProductAccumulator & operator*=( const Scal z )
        {
            Int shift = 0;
            
            if constexpr ( Scalar::RealQ<Scal> )
            {
                std::tie(x,shift) = mantissa_exponent( x * Re(z) );
                e += shift;
            }
            else
            {
                std::tie(x,y,shift) = mantissa_exponent( Complex(x,y) * z );
                e += shift;
            }
            
            return *this;
        }
        
        
        Scal Mantissa2() const
        {
            return Complex(x,y);
        }
        
        Int Exponent2() const
        {
            return e;
        }
        
        std::pair<Scal,Int> MantissaExponent2()  const
        {
            if constexpr ( Scalar::RealQ<Scal> )
            {
                return std::pair<Real,Int>( x, e );
            }
            else
            {
                return std::pair<Complex,Int>( Complex(x,y), e );
            }
        }
        
        Scal Mantissa10() const
        {
            return MantissaExponent10().first;
        }
        
        Int Exponent10() const
        {
            return MantissaExponent10().second;
        }
        
        std::pair<Scal,Int> MantissaExponent10()  const
        {
//            constexpr Real conversion_factor = std::log(Real(2)) / std::log(Real(10));
            constexpr Real conversion_factor = 0.3010299956639812;
            
            auto exp10 = []( const Real x_ )
            {
                return std::pow( Real(10), x_ );
            };
            
            if constexpr ( Scalar::RealQ<Scal> )
            {
                Real exponent10 = e * conversion_factor;
                Int  e10 = std::trunc(exponent10);
                Real x10 = x * exp10( exponent10 - e10 );

                while( Abs(x10) < Real(1) )
                {
                    x10 *= Real(10);
                    ++e10;
                }
                
                while( Abs(x10) >= Real(10) )
                {
                    x10 *= Frac<Real>(1,10);
                    ++e10;
                }
                
                return std::pair<Real,Int>( x10, e10 );
            }
            else
            {
                Real    exponent10 = e * conversion_factor;
                Int     e10 = std::trunc(exponent10);
                Complex z10 = Complex(x,y) * exp10( exponent10 - e10 );
                
                while( AbsSquared(z10) < Real(1) )
                {
                    z10 *= Real(10);
                    --e10;
                }
                
                while( AbsSquared(z10) >= Real(100) )
                {
                    z10 *= Frac<Real>(1,10);
                    ++e10;
                }
                
                return std::pair<Complex,Int>( z10, e10 );
            }
        }
    }; // class ProduceAccumulator
    
} // namespace Tools
