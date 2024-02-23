#pragma once

namespace Tools
{

    template<typename Scal, typename Int>
    [[nodiscard]] Scal Det_Bareiss( const Int n, mptr<Scal> A_, const Int ldA ) const
    {
        ASSERT_INT(Int);
        
        auto A = [A_,ldA]( const Int i, const Int j) -> Scal &
        {
            return A_[ldA * i + j];
        }
        
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
                //Pivot - row swap needed
                if( A(k,k) == zero )
                {
                    const Int l = iamax_buffer( &A(k,k+1), n-k-1  );
                    
                    for( Int j = 0; j < n; ++j )
                    {
                        std::swap( A(k,j), A(l,j) );
                    }
                    
                    if( A(k,k) == zero )
                    {
                        return zero;
                    };
                    
//                    Int l;
//                    
//                    for( l = k + 1; l < n; ++l )
//                    {
//                        // TODO: We could find a better pivot...
//                        if( A(l,k) != zero )
//                        {
//                            std::swap_ranges( &A(l,0), &A(l,n), &A(k,0) );
//                            sign = -sign;
//                            break;
//                        }
//                    }
//                    
//                    //No entries != 0 found in column k -> det = 0
//                    if(l == n)
//                    {
//                        return zero;
//                    }
                }
                
                const Scal A_k_k = A(k  ,k  );
                const Scal a     = A(k-1,k-1);
                
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
