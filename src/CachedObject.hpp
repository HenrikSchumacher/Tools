#pragma once

#include <string>
#include <type_traits>
#include <any>
#include <unordered_map>

namespace Tools
{
    
    class CachedObject
    {
    public:
        
        using Container_T = std::unordered_map<std::string,std::any>;
        
        CachedObject() = default;
        
        ~CachedObject() = default;
        
        // TODO: We might have to do deep copies in the following 4 if we store pointers...
        
        /* Copy constructor */
        CachedObject(const CachedObject & rhs)
        :     cache( rhs.cache )
        ,   p_cache( rhs.p_cache )
        {}
        
        /* Move constructor */
        CachedObject( CachedObject && rhs) noexcept
        :   cache   ( std::move(rhs.cache   ) )
        ,   p_cache ( std::move(rhs.p_cache ) )
        {}
        
        // TODO: Why return a const reference here?
        /* Copy assignment */
        const CachedObject & operator=( const CachedObject & rhs)
        {
            cache   = rhs.cache;
            p_cache = rhs.p_cache;
            
            return *this;
        };

        // TODO: Why return a const reference here?
        /* Move assignment */
        const CachedObject & operator=( CachedObject && rhs) noexcept
        {
            cache   = std::move(rhs.cache);
            p_cache = std::move(rhs.p_cache);
            
            return *this;
        };
        
    protected:
            
        mutable Container_T cache;
        
        mutable std::mutex cache_mutex;
        
        mutable Container_T p_cache;
        
        mutable std::mutex p_cache_mutex;
        
    public:
        
//##########################################################################################
//      Cache
//##########################################################################################
        
        bool InCacheQ( cref<std::string> key ) const
        {
            // For some reason gcc-12 does not allow me to return static_cast<bool>( cache.count(key) ) directly.
            
            bool result = false;
            
//            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            result = static_cast<bool>( cache.count( key ) );
            
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
                throw; //an internal catch block forwards the exception to its external level
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
                if( InCacheQ(key) )
                {
                    wprint( this->ClassName() + "::SetCache: Key " + key + " is already in cache. Check for race conditions." );
                }
            }
            
            cache[key] = std::move(thing);
        }
        
        std::string CacheKeys() const
        {
            std::stringstream s;
            
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            s << "{ \n";
            for( auto const & p : cache )
            {
                s << "\t" << p.first << "\n";
            }
            s << "}";
            return s.str();
        }
        
        void ClearCache() const
        {
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            cache.clear();
        }
        
        void ClearCache( const std::string & key ) const
        {
            if( InCacheQ( key ) )
            {
                const std::lock_guard<std::mutex> cache_lock( cache_mutex );
                
                cache.erase( key );
            }
        }
        
        
//##########################################################################################
//      PersistentCache
//##########################################################################################
        
        bool InPersistentCacheQ( cref<std::string> key ) const
        {
            // For some reason gcc-12 does not allow me to return static_cast<bool>( cache.count(key) ) directly.
            
            bool result = false;
            
//            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            result = static_cast<bool>( p_cache.count( key ) );
            
            return result;
        }
        
        template<typename T = std::any>
        T & GetPersistentCache( cref<std::string> key ) const
        {
            // For some reason gcc-12 does not allow me to return cache.at(key) directly. =/
            // Maybe because that might throw an exception.
            
            std::any * thing;
            
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            try
            {
                thing = &p_cache.at( key );
            }
            catch( const std::out_of_range & e )
            {
                eprint(this->ClassName()+"GetPersistentCache: Key \""+key+"\" not found!.");
                throw; //an internal catch block forwards the exception to its external level
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
                if( InCacheQ(key) )
                {
                    wprint( this->ClassName() + "::SetPersistentCache: Key " + key + " is already in cache. Check for race conditions." );
                }
            }
            
            p_cache[key] = std::move(thing);
        }
        
        std::string PersistentCacheKeys() const
        {
            std::stringstream s;
            
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            s << "{ \n";
            for( auto const & p : p_cache )
            {
                s << "\t" << p.first << "\n";
            }
            s << "}";
            return s.str();
        }
        
        void ClearPersistentCache() const
        {
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            p_cache.clear();
        }
        
        void ClearPersistentCache( cref<std::string> key ) const
        {
            if( InPersistentCacheQ( key ) )
            {
                const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
                
                p_cache.erase( key );
            }
        }
        
        
        
//##########################################################################################
//      Misc
//##########################################################################################
        
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
