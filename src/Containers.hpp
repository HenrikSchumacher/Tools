#pragma once

#ifdef TOOLS_USE_BOOST_UNORDERED
    #include <boost/unordered/unordered_flat_map.hpp>
    #include <boost/unordered/unordered_flat_set.hpp>
#else
    #include <unordered_map>
    #include <unordered_set>
#endif

namespace Tools
{
#ifdef TOOLS_USE_BOOST_UNORDERED
    template<typename Key_T, typename Hash_T = boost::hash<Key_T>>
    using SetContainer = boost::unordered_flat_set<Key_T,Hash_T>;
#else
    template<typename Key_T, typename Hash_T = Tools::Hash<Key_T>>
    using SetContainer = std::unordered_set<Key_T,Hash_T>;
#endif
    
    template<typename Key_T, typename Hash_T = Tools::Hash<Key_T>>
    std::string ToString( cref<SetContainer<Key_T,Hash_T>> set )
    {
        std::string s ("{ ");
        
        const Size_T n = set.size();
        
        Size_T iter = 0;
        
        for( auto & x : set )
        {
            ++iter;
            s += ToString(x);
            if( iter < n )
            {
                s += ", ";
            }
        }
        
        s += " }";
        
        return s;
    }
    
    
#ifdef TOOLS_USE_BOOST_UNORDERED
    template<typename Key_T, typename Val_T, typename Hash_T = boost::hash<Key_T>>
    using AssociativeContainer = boost::unordered_flat_map<Key_T,Val_T,Hash_T>;

#else
    template<typename Key_T, typename Val_T, typename Hash_T = Hash<Key_T>>
    using AssociativeContainer = std::unordered_map<Key_T,Val_T,Hash_T>;
#endif
    
    template<typename Key_T, typename Val_T, typename Hash_T>
    std::string ToString( cref<AssociativeContainer<Key_T,Val_T,Hash_T>> a )
    {
        std::string s ("{ ");
        
        const Size_T n = a.size();
        
        Size_T iter = 0;
        
        for( auto & x : a )
        {
            ++iter;
            s += ToString(x.first) + " -> " + ToString(x.second);
            if( iter < n )
            {
                s += ", ";
            }
        }
        
        s += " }";
        
        return s;
    }
    
    template<typename Key_T, IntQ Val_T, typename Hash_T>
    TOOLS_FORCE_INLINE void Increment(
        mref<AssociativeContainer<Key_T,Val_T,Hash_T>> a, cref<Key_T> key, cref<Val_T> val
    )
    {
        if( a.contains(key) )
        {
            a[key] += val;
        }
        else
        {
            a[key]  = val;
        }
    }
    
    template<typename Key_T, IntQ Val_T, typename Hash_T>
    TOOLS_FORCE_INLINE void Increment(
        mref<AssociativeContainer<Key_T,Val_T,Hash_T>> a, cref<Key_T> key
    )
    {
        if( a.contains(key) )
        {
            Val_T val = a[key];
            
            if( val == Val_T(-1))
            {
                a.erase(key);
            }
            else
            {
                ++val;
                a[key] = val;
            }
        }
        else
        {
            a[key] = Val_T(1);
        }
    }
    
    template<typename Key_T, IntQ Val_T, typename Hash_T>
    TOOLS_FORCE_INLINE void Decrement(
        mref<AssociativeContainer<Key_T,Val_T,Hash_T>> a, cref<Key_T> key
    )
    {
        if( a.contains(key) )
        {
            Val_T val = a[key];
            
            if( val == Val_T(1))
            {
                a.erase(key);
            }
            else
            {
                --val;
                a[key] = val;
            }
        }
        else
        {
            a[key] = Val_T(-1);
        }
    }
        
    #ifdef LTEMPLATE_H
    
        template<mma::HasTypeQ S, typename Hash_T>
        inline mma::TensorRef<mma::Type<S>> to_MTensorRef( cref<SetContainer<S,Hash_T>> source )
        {
            using T = mma::Type<S>;
            auto r = mma::makeVector<T>( int_cast<mint>(source.size()) );
            mptr<T> target = r.data();
            
            mint i = 0;
            
            for( auto & x : source )
            {
                if constexpr ( (SameQ<S,double> || SameQ<S,float>) )
                {
                    if( x == Scalar::Infty<S> )
                    {
                        target[i] = Scalar::Max<T>;
                    }
                    else if( x == -Scalar::Infty<S> )
                    {
                        target[i] = -Scalar::Max<T>;
                    }
                    else
                    {
                        target[i] = static_cast<T>(x);
                    }
                }
                else
                {
                    target[i] = static_cast<T>(x);
                }
                ++i;
            }
            
            return r;
        }
        
        
        template<mma::HasTypeQ S>
        inline mma::TensorRef<mma::Type<S>> to_MTensorRef( cref<std::vector<S>> source )
        {
            using T = mma::Type<S>;
            auto r = mma::makeVector<T>( int_cast<mint>(source.size()) );
            copy_buffer( &source[0], r.data(), source.size() );
            return r;
        }
    
        template<mma::HasTypeQ S, std::size_t n>
        inline mma::TensorRef<mma::Type<S>> to_MTensorRef( cref<std::array<S,n>> source )
        {
            using T = mma::Type<S>;
            auto r = mma::makeVector<T>( int_cast<mint>(n) );
            copy_buffer( &source[0], r.data(), n );
            return r;
        }
        
    #endif // LTEMPLATE_H
    
} // namespace Tools
