#pragma once

namespace Tools
{
    
    class CachedObject
    {
    public:
        
        using Container_T = std::unordered_map<std::string,std::any>;
        
        CachedObject() = default;
        
        ~CachedObject() = default;
        
    protected:
            
        mutable Container_T cache;
        
        mutable Container_T persistent_cache;
        
    public:
        
//##############################################################################################
//      Cache
//##############################################################################################
        
        bool InCacheQ( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return static_cast<bool>( cache.count(s) ) directly.
            
            bool result = false;
            
            #pragma omp critical (cache)
            {
                result = static_cast<bool>( cache.count(s) );
            }
            
            return result;
        }
        
        std::any & GetCache( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return cache.at(s) directly. =/
            // Maybe because that might throw an exception.
            
            std::any * thing;
            
            #pragma omp critical (cache)
            {
                try
                {
                    thing = &cache.at(s);
                }
                catch( const std::out_of_range & e )
                {
                    eprint(this->ClassName()+"GetCache: Key \""+s+"\" not found!.");
                    throw; //an internal catch block forwards the exception to its external level
                }
            }
            
            return *thing;
        }
        
        // Caution! This function is destructive.
        void SetCache( const std::string & s, std::any & thing ) const
        {
            #pragma omp critical (cache)
            {
                cache[s] = std::move(thing);
            }
        }
        
        std::string CacheKeys() const
        {
            std::stringstream s;
            
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
            cache = Container_T();
        }
        
        
//##############################################################################################
//      PersistentCache
//##############################################################################################
        
        bool InPersistentCacheQ( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return static_cast<bool>( cache.count(s) ) directly.
            
            bool result = false;
            
            #pragma omp critical (cache)
            {
                result = static_cast<bool>( persistent_cache.count(s) );
            }
            
            return result;
        }
        
        std::any & GetPersistentCache( const std::string & s ) const
        {
            // For some reason gcc-12 does not allow me to return cache.at(s) directly. =/
            // Maybe because that might throw an exception.
            
            std::any * thing;
            
            #pragma omp critical (cache)
            {
                try
                {
                    thing = &persistent_cache.at(s);
                }
                catch( const std::out_of_range & e )
                {
                    eprint(this->ClassName()+"GetPersistentCache: Key \""+s+"\" not found!.");
                    throw; //an internal catch block forwards the exception to its external level
                }
            }
            
            return *thing;
        }
        
        // Caution! This function is destructive.
        void SetPersistentCache( const std::string & s, std::any & thing ) const
        {
            #pragma omp critical (cache)
            {
                persistent_cache[s] = std::move(thing);
            }
        }
        
        std::string PersistentCacheKeys() const
        {
            std::stringstream s;
            
            s << "{ \n";
            for( auto const & p : persistent_cache )
            {
                s << "\t" << p.first << "\n";
            }
            s << "}";
            return s.str();
        }
        
        void ClearPersistentCache() const
        {
            persistent_cache = Container_T();
        }
        
        
//##############################################################################################
//      Misc
//##############################################################################################
        
        void ClearAllCache() const
        {
            cache = Container_T();
            
            persistent_cache = Container_T();
        }
        
        
        virtual std::string ClassName() const
        {
            return std::string("CachedObject");
        }
    
    } // CachedObject
    
} // namespace Tools
