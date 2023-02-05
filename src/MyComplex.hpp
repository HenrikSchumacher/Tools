#pragma once

namespace Tools
{
 
    template<typename Real_>
    struct MyComplex
    {
        using Real = Real_;
        
    private:
        
        Real z[2];
        
    public:
        
        ~MyComplex() = default;

        // TODO: Casting constructors.
        
        MyComplex ( const Real re_  )
        :   z { re_, static_cast<Real>(0) }
        {}
        
        MyComplex ( const Real re_, const Real im_  )
        :   z { re_, im_ }
        {}
        
//        MyComplex ( const MyComplex & z_ )
//        :   z { z.z[0], z.z[1] }
//        {}
        

        
        force_inline constexpr Real & Re()
        {
            return z[0];
        }
        
        force_inline constexpr const Real & Re() const
        {
            return z[0];
        }
        
        force_inline constexpr Real & Im()
        {
            return z[1];
        }
        
        force_inline constexpr const Real & Im() const
        {
            return z[1];
        }
        
        friend force_inline constexpr const MyComplex operator+(
            const MyComplex & x, const MyComplex & y
        )
        {
            return Complex( x.z[0] + y.z[0], x.z[1] + y.z[1] );
        }
        
        friend force_inline constexpr const MyComplex operator-(
            const MyComplex & x, const MyComplex & y
        )
        {
            return Complex( x.z[0] - y.z[0], x.z[1] - y.z[1] );
        }
        
        friend force_inline constexpr const MyComplex operator*(
            const MyComplex & x, const MyComplex & y
        )
        {
            return Complex(
                x.z[0] * y.z[0] - x.z[1] + y.z[1],
                x.z[0] * y.z[1] + x.z[1] + y.z[0]
            );
        }
        
    };
}
