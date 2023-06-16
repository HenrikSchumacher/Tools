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
        
        CachedObject(const CachedObject & rhs)
        :     cache( rhs.cache )
        ,   p_cache( rhs.p_cache )
        {}
        
        CachedObject( CachedObject && rhs)
        :   cache( rhs.cache )
        ,   p_cache( rhs.p_cache )
        {}
        
        const CachedObject & operator=( const CachedObject & rhs)
        {
            cache   = rhs.cache;
            p_cache = rhs.p_cache;
            
            return *this;
        };

        const CachedObject & operator=( CachedObject && rhs)
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
        
//##############################################################################################
//      Cache
//##############################################################################################
        
        bool InCacheQ( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return static_cast<bool>( cache.count(s) ) directly.
            
            bool result = false;
            
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            result = static_cast<bool>( cache.count(s) );
            
            return result;
        }
        
        std::any & GetCache( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return cache.at(s) directly. =/
            // Maybe because that might throw an exception.
            
            std::any * thing;
            
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            try
            {
                thing = &cache.at(s);
            }
            catch( const std::out_of_range & e )
            {
                eprint(this->ClassName()+"GetCache: Key \""+s+"\" not found!.");
                throw; //an internal catch block forwards the exception to its external level
            }
            
            return *thing;
        }
        
//        // Caution! This function is destructive.
//        void SetCache( const std::string & s, std::any & thing ) const
//        {
//            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
//            cache[s] = std::move(thing);
//        }
        
        // Caution! This function is destructive.
        void SetCache( const std::string & s, std::any && thing ) const
        {
            const std::lock_guard<std::mutex> cache_lock( cache_mutex );
            
            cache[s] = std::move(thing);
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
            
            cache = Container_T();
        }
        
        
//##############################################################################################
//      PersistentCache
//##############################################################################################
        
        bool InPersistentCacheQ( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return static_cast<bool>( cache.count(s) ) directly.
            
            bool result = false;
            
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            result = static_cast<bool>( p_cache.count(s) );
            
            return result;
        }
        
        std::any & GetPersistentCache( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return cache.at(s) directly. =/
            // Maybe because that might throw an exception.
            
            std::any * thing;
            
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            try
            {
                thing = &p_cache.at(s);
            }
            catch( const std::out_of_range & e )
            {
                eprint(this->ClassName()+"GetPersistentCache: Key \""+s+"\" not found!.");
                throw; //an internal catch block forwards the exception to its external level
            }
            
            return *thing;
        }
        
        // Caution! This function is destructive.
        void SetPersistentCache( const std::string & s, std::any && thing ) const
        {
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            p_cache[s] = std::move(thing);
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
            
            p_cache = Container_T();
        }
        
        
//##############################################################################################
//      Misc
//##############################################################################################
        
        void ClearAllCache() const
        {
            const std::lock_guard<std::mutex>   cache_lock(   cache_mutex );
            const std::lock_guard<std::mutex> p_cache_lock( p_cache_mutex );
            
            cache = Container_T();
            
            p_cache = Container_T();
        }
        
        
        std::string ClassName() const
        {
            return std::string("CachedObject");
        }
    
    }; // CachedObject
    
} // namespace Tools
