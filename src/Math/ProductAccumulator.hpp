#pragma once

namespace Tools
{
    template<typename Scal_, IntQ Int_ = Int64>
    class ProductAccumulator final
    {
        using Scal    = Scal_;
        using Int     = Int_;
        using Real    = Scalar::Real<Scal>;
        using Complex = Scalar::Complex<Real>;
        
        static_assert( Scalar::FloatQ<Real>, "" );
        static_assert( Scalar::RealQ<Scal> || Scalar::ComplexQ<Scal>, "" );
        
    private:
        
        Scal m = 0.5; // mantissa
        Int  e = 1;   // exponent
        
    public:
        
        ProductAccumulator() = default;
        
        ProductAccumulator( cref<Scal> init )
        {
            std::tie(m,e)   = mantissa_exponent(init);
        }
        
        ProductAccumulator( cref<Scal>  mantissa_init, cref<Int> exponent_init )
        :   ProductAccumulator( mantissa_init )
        {
            e += exponent_init;
        }

        ~ProductAccumulator() = default;
        
        
    private:
        
        static std::tuple<Real,Int> mantissa_exponent( const Real z )
        {
            int exponent;
            Real mantissa = std::frexp( z, &exponent );
            
            return std::tuple<Real,Int>(mantissa,exponent);
        }
        
        static std::tuple<Complex,Int> mantissa_exponent( const Complex z )
        {
            auto [u,e_u] = mantissa_exponent( Re(z) );
            auto [v,e_v] = mantissa_exponent( Im(z) );
            
            const Int d = e_u - e_v;
            
            Int exponent;
            
            const bool use_u_e = ((d >= Int(0)) && (u != Int(0))) || (v == Int(0));
            
            if( use_u_e )
            {
                exponent = e_u;
                v = v * Tools::Power(Real(2),-d);
            }
            else
            {
                exponent = e_v;
                u = u * Tools::Power(Real(2),d);
            }
            
            return std::tuple<Complex,Int>({u,v},exponent);
        }
        
    public:
        
        ProductAccumulator & operator*=( const Scal z )
        {
            Int shift;
            std::tie(m,shift) = mantissa_exponent(m * z);
            e += shift;
            return *this;
        }
        
        ProductAccumulator & operator*=( const ProductAccumulator & z )
        {
            e += z.e;
            return (*this) *= z.m;
        }
        
        
        friend ProductAccumulator operator*(
            const ProductAccumulator & a, const ProductAccumulator & b
        )
        {
            ProductAccumulator c (a);
            c *= b;
            return c;
        }
        
        ProductAccumulator & operator/=( const Scal z )
        {
            Int shift = 0;
            std::tie(m,shift) = mantissa_exponent(m / z);
            e += shift;
            return *this;
        }
        
        ProductAccumulator & operator/=( const ProductAccumulator & z )
        {
            e -= z.Exponent2();
            return (*this) /= z.Mantissa2();
        }

        
        friend ProductAccumulator operator/(
            const ProductAccumulator & a, const ProductAccumulator & b
        )
        {
            ProductAccumulator c (a);
            c /= b;
            return c;
        }
        
        friend ProductAccumulator<Real,Int> Abs( const ProductAccumulator<Scal,Int> z )
        {
            return Abs(z.Mantissa2()) * ProductAccumulator<Real,Int>::Power(Real(2),z.Exponent2());
        }
        
        ProductAccumulator Power( const Int exponent ) const
        {
            if constexpr ( Scalar::RealQ<Scal> )
            {
                Real log2_b = std::log2(m) * static_cast<Real>(exponent);
                
                // TODO: Warning: potentially undefined behavior if log2_b us not finite.
                Real q_Real = std::trunc(log2_b);
                Int  q      = static_cast<Int>(q_Real);

                ProductAccumulator<Real,Int> b;
                b.m = std::exp2(log2_b - q_Real);
                b.e = e * exponent + q;
                
                // Normalization step. Maybe obsolete.
                int delta;
                
                b.m  = std::frexp( b.m, &delta );
                b.e += delta;
                
                return b;
                
            }
            else
            {
                // magnitude of mantissa
                ProductAccumulator<Real,Int> r = Abs(*this);
                
                if( r.Mantissa2() == Real(0) )
                {
                    return ProductAccumulator<Complex,Int>(0);
                }
                
                // magnitude of mantissa^exponent
                ProductAccumulator<Real,Int> R = r.Power(exponent);
                
                // argument of mantissa^exponent
                Real Theta = static_cast<Real>(exponent) * std::atan2(Im(m),Re(m));
                
                ProductAccumulator<Complex,Int> b;
                
                b.m = Complex{R.Mantissa2() * std::cos(Theta), R.Mantissa2() * std::sin(Theta)};
                b.e = R.Exponent2();
                
                return b;
            }
            
        }
        
        static ProductAccumulator Power( const Scal & base, Int e )
        {
            ProductAccumulator b (base);
            return b.Power(e);
        }
        
        Scal Mantissa2() const
        {
            return m;
        }
        
        Int Exponent2() const
        {
            return e;
        }
        
        std::pair<Scal,Int> MantissaExponent2()  const
        {
            return std::pair<Scal,Int>(m, e);
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
            // An infinite mantissa would never leave the scaling loops below (inf * 0.1 == inf).
            
            if( !FiniteQ(m) )
            {
                return {Mantissa2(),Int{0}};
            }

            if( m == Scal{0} )
            {
                return {Scal{0},Int{0}};
            }
            
//            constexpr Real conversion_factor = std::log(Real(2)) / std::log(Real(10));
            constexpr Real conversion_factor = 0.3010299956639812;
            
            auto exp10 = []( const Real x_ )
            {
                return std::pow( Real{10}, x_ );
            };
            
            Real exponent10 = static_cast<Real>(e) * conversion_factor;
            Real e10_Real   = std::trunc(exponent10);
            Int  e10        = static_cast<Int>(e10_Real);
            Scal m10        = m * exp10( exponent10 - e10_Real );
            
            // If RealQ<Scal>, then Abs is probably faster than AbsSquared.
            // But if ComplexQ<Scal>, then AbsSquared will be faster.
            
            if constexpr ( Scalar::RealQ<Scal> )
            {
                while( Abs(m10) < Real{1} )
                {
                    m10 *= Real{10};
                    --e10;
                }
                
                while( Abs(m10) >= Real{10} )
                {
                    m10 *= Frac<Real>(1,10);
                    ++e10;
                }
            }
            else
            {
                while( AbsSquared(m10) < Real{1} )
                {
                    m10 *= Real{10};
                    --e10;
                }
                
                while( AbsSquared(m10) >= Real{100} )
                {
                    m10 *= Frac<Real>(1,10);
                    ++e10;
                }
            }
            
            return {m10, e10};
        }


        [[nodiscard]] friend std::string ToString( cref<ProductAccumulator> a ) 
        {
            return OutString::FromMisc("{ ", a.m,  ", ", a.e, " } ");
        }
        
    }; // class ProduceAccumulator
    
} // namespace Tools
