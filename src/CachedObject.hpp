#pragma once

#include <string>
#include <type_traits>
#include <any>
#include <unordered_map>

namespace Tools
{
    
    class alignas( ObjectAlignment ) CachedObject
    {
    public:
        
        using Container_T = std::unordered_map<std::string,std::any>;
        
        
// For move semantics for mutex-locked classes see here:
// https://stackoverflow.com/a/29988626/8248900
        
        using Mutex_T     = std::mutex;
        using ReadLock_T  = std::unique_lock<Mutex_T>;
        using WriteLock_T = std::unique_lock<Mutex_T>;
        
        // Default constructor
        CachedObject() = default;
        
        // Destructor (virtual because this class is inherited by many others)
        virtual ~CachedObject() = default;
        
        // Copy constructor
        CachedObject( const CachedObject & other ) noexcept
        {
            {
                ReadLock_T  rhs_lk( other.cache_mutex );
                this->cache   = other.cache;
            }
            {
                ReadLock_T  rhs_lk( other.p_cache_mutex );
                this->p_cache = other.p_cache;
            }
        }
        
        // Move constructor
        CachedObject( CachedObject && other) noexcept
        {
            {
                WriteLock_T rhs_lk ( other.cache_mutex   );
                this->cache   = std::move( other.cache   );
            }
            
            {
                WriteLock_T rhs_lk ( other.p_cache_mutex );
                this->p_cache = std::move( other.p_cache );
            }
        }
        
        // Copy assignment
        CachedObject & operator=(const CachedObject & other ) noexcept
        {
            if( this != &other )
            {
                {
                    WriteLock_T lhs_lk( this->cache_mutex, std::defer_lock );
                    ReadLock_T  rhs_lk( other.cache_mutex, std::defer_lock );
                    std::lock( lhs_lk, rhs_lk );
                    this->cache = other.cache;
                }
                
                {
                    WriteLock_T lhs_lk( this->p_cache_mutex, std::defer_lock );
                    ReadLock_T  rhs_lk( other.p_cache_mutex, std::defer_lock );
                    std::lock( lhs_lk, rhs_lk );
                    this->p_cache = other.p_cache;
                }
            }
            return *this;
        }
        
        // Move assignment
        CachedObject & operator=( CachedObject && other ) noexcept
        {
            if (this != &other)
            {
                {
                    WriteLock_T lhs_lk ( this->cache_mutex, std::defer_lock );
                    WriteLock_T rhs_lk ( other.cache_mutex, std::defer_lock );
                    std::lock ( lhs_lk, rhs_lk );
                    this->cache = std::move( other.cache   );
                }
                
                {
                    WriteLock_T lhs_lk ( this->p_cache_mutex, std::defer_lock );
                    WriteLock_T rhs_lk ( other.p_cache_mutex, std::defer_lock );
                    std::lock ( lhs_lk, rhs_lk );
                    this->p_cache = std::move( other.p_cache   );
                }
            }
            return *this;
        }
        
        // Swap function
        friend void swap( CachedObject & x, CachedObject & y ) noexcept
        {
            if( &x != &y )
            {
                {
                    WriteLock_T lhs_lk( x.cache_mutex, std::defer_lock );
                    WriteLock_T rhs_lk( y.cache_mutex, std::defer_lock );
                    std::lock( lhs_lk, rhs_lk );
                    using std::swap;
                    swap( x.cache, y.cache );
                }
                
                {
                    WriteLock_T lhs_lk( x.p_cache_mutex, std::defer_lock );
                    WriteLock_T rhs_lk( y.p_cache_mutex, std::defer_lock );
                    std::lock( lhs_lk, rhs_lk );
                    using std::swap;
                    swap( x.p_cache, y.p_cache );
                }
            }
        }
        
    protected:
            
        mutable Container_T cache;
        
        mutable Mutex_T cache_mutex;
        
        mutable Container_T p_cache;
        
        mutable Mutex_T p_cache_mutex;
        
    public:
        
//###########################################################
//      Cache
//###########################################################
        
        template<bool lockQ = true>
        bool InCacheQ( cref<std::string> key ) const
        {
            bool result = false;
            
            if constexpr( lockQ )
            {
                const std::lock_guard<std::mutex> cache_lock( cache_mutex );
                result = cache.contains( key );
            }
            else
            {
                // The version without lock is necessary because we call InCacheQ frequently from locked routines.
                result = cache.contains( key );
            }
            
            return result;
        }
        
        template<typename T = std::any>
        T & GetCache( cref<std::string> key ) const
        {
            // For some reason gcc-12 does not allow me to return cache.at(key) directly. =/
            // Maybe because that might throw an exception.
            
            std::any * thing;
            
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            try
            {
                thing = &cache.at( key );
            }
            catch( const std::out_of_range & e )
            {
                eprint(this->ClassName()+"::GetCache: Key \""+key+"\" not found!.");
                throw(e); //an internal catch block forwards the exception to its external level
            }
            
            if constexpr ( std::is_same_v<T,std::any> )
            {
                return *thing;
            }
            else
            {
                return std::any_cast<T &>(*thing);
            }
        }
        
//        // Caution! This function is destructive.
//        void SetCache( cref<std::string> key, std::any & thing ) const
//        {
//            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
//            cache[key] = std::move(thing);
//        }
        
        // Caution! This function is destructive.
        template<bool check_existence_Q = true>
        void SetCache( cref<std::string> key, std::any && thing ) const
        {
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            if constexpr ( check_existence_Q )
            {
                if( this->template InCacheQ<false>(key) )
                {
                    wprint( this->ClassName()+"::SetCache: Key \"" + key + "\" is already in cache. Maybe check for race conditions." );
                }
            }
            
            cache[key] = std::move(thing);
        }
        
        std::string CacheKeys() const
        {
            std::string s;
            
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            s += "{\n";
            for( auto const & p : cache )
            {
                s += "\t";
                s += p.first;
                s += "\n";
            }
            s += "}";
            return s;
        }
        
        void ClearCache() const
        {
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            if( !cache.empty() )
            {
                cache.clear();
            }
        }
        
        void ClearCache( const std::string & key ) const
        {
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            if( this->template InCacheQ<false>( key ) )
            {
                cache.erase( key );
            }
        }
        
        
//###########################################################
//      PersistentCache
//###########################################################
        
        template<bool lockQ = true>
        bool InPersistentCacheQ( cref<std::string> key ) const
        {
            bool result = false;
            
            if constexpr( lockQ )
            {
                const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
                result = p_cache.contains( key );
            }
            else
            {
                result = p_cache.contains( key );
            }
            return result;
        }
        
        template<typename T = std::any>
        T & GetPersistentCache( cref<std::string> key ) const
        {
            std::any * thing;
            
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            try
            {
                thing = &p_cache.at( key );
            }
            catch( const std::out_of_range & e )
            {
                eprint(this->ClassName()+"GetPersistentCache: Key \""+key+"\" not found!.");
                throw(e);
            }
            
            if constexpr ( std::is_same_v<T,std::any> )
            {
                return *thing;
            }
            else
            {
                return std::any_cast<T &>(*thing);
            }
        }
        
        // Caution! This function is destructive.
        template<bool check_existence_Q = true>
        void SetPersistentCache( cref<std::string> key, std::any && thing ) const
        {
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            if constexpr ( check_existence_Q )
            {
                if( this->template InPersistentCacheQ<false>(key) )
                {
                    wprint( this->ClassName()+"::SetPersistentCache: Key \"" + key + "\" is already in cache. Maybe check for race conditions." );
                }
            }
            
            p_cache[key] = std::move(thing);
        }
        
        std::string PersistentCacheKeys() const
        {
            std::string s;
            
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            s += "{\n";
            for( auto const & p : p_cache )
            {
                s += "\t";
                s += p.first;
                s += "\n";
            }
            s += "}";
            return s;
        }
        
        void ClearPersistentCache() const
        {
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
         
            if( !p_cache.empty() )
            {
                p_cache.clear();
            }
        }
        
        void ClearPersistentCache( cref<std::string> key ) const
        {
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            if( this->template InPersistentCacheQ<false>( key ) )
            {
                p_cache.erase( key );
            }
        }
        
        
        
//###########################################################
//      Misc
//###########################################################
        
        void ClearAllCache() const
        {
            const std::lock_guard<std::mutex>   cache_lock(   cache_mutex );
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            cache.clear();
            
            p_cache.clear();
        }
        
        
        std::string ClassName() const
        {
            return std::string("CachedObject");
        }
    
    }; // CachedObject
    
} // namespace Tools
