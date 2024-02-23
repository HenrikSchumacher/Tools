#pragma once

namespace Tools
{

    template<typename Scal, typename Int>
    [[nodiscard]] Scal Det_Bareiss( const Int n, mptr<Scal> A_, const Int ldA )
    {
        ASSERT_INT(Int);
        
        constexpr Scal zero ( 0 );
        
        auto A = [A_,ldA]( const Int i, const Int j) -> Scal &
        {
            return A_[ldA * i + j];
        };
        
        if( n == 0 )
        {
            return 1;
        }
        
        if( n == 1 )
        {
            return A(0,0);
        }
        
        if( n == 2 )
        {
            return A(0,0) * A(1,1) - A(0,1) * A(1,0);
        }
        
        if( n == 3 )
        {
            return (
                  A(0,0) * A(1,1) * A(2,2)
                + A(0,1) * A(1,2) * A(2,0)
                + A(0,2) * A(1,0) * A(2,1)
                - A(0,0) * A(1,2) * A(2,1)
                - A(0,1) * A(1,0) * A(2,2)
                - A(0,2) * A(1,1) * A(2,0)
            );
        }
        
        // Bareiss algorithm copied and adapted from https://cs.stackexchange.com/q/124759/146040
        
        if( n > 3 )
        {
            signed char sign ( 1 );
            
            for(Int k = 0; k < n - 1; ++k )
            {
                //Pivot column swap.
                
                const Int l = k + iamax_buffer( &A(k,k), n-k );
                
                if( l != k )
                {
                    sign = -sign;
                    
                    for( Int i = 0; i < n; ++i )
                    {
                        std::swap(A(i,k),A(i,l));
//                        const Scal buffer = A(k,j);
//                        A(k,j) = A(l,j);
//                        A(l,j) = buffer;
                    }
                }
                
                const Scal A_k_k = A(k,k);
                
                if( A_k_k == zero )
                {
                    return zero;
                };
                    
                
                const Scal a  = A(k-1,k-1);
                
                // TODO: When not working with integers, we probably want to compute 1/a only once.
                
                //Apply formula
                for( Int i = k + 1; i < n; ++i )
                {
                    for( Int j = k + 1; j < n; ++j )
                    {
                        A(i,j) = A_k_k * A(i,j) - A(i,k) * A(k,j);

                        if(k != 0)
                        {
                            A(i,j) /= a;
                        }
                    }
                }
            }
            
            return sign > 0 ? A(n-1,n-1) : -A(n-1,n-1);
        }
        
        return 0;
    }
}
