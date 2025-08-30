#pragma once

namespace Tools {
    
    // https://stackoverflow.com/a/9729747/8248900

    template <class T>
    inline void hash_combine(std::size_t & seed, const T & x)
    {
        using namespace std;
        hash<T> hasher;
        seed ^= hasher(x) + std::size_t(0x9e3779b9) + (seed << 6) + (seed >> 2);
    }

    template<typename S, typename T>
    struct pair_hash
    {
        inline std::size_t operator()(const std::pair<S,T> & p) const
        {
            std::size_t seed = 0;
            hash_combine(seed, p.first);
            hash_combine(seed, p.second);
            return seed;
        }
    };
    
    
    struct array_hash
    {
        template<typename T, std::size_t n>
        inline std::size_t operator()(const std::array<T,n> & a) const
        {
            std::size_t seed = 0;
            for( std::size_t i = 0; i < n; ++i )
            {
                hash_combine(seed, a[i]);
            }
            return seed;
        }
    };

    
} // namespace Tools
