#pragma once

namespace Tools {
    
    // https://stackoverflow.com/a/9729747/8248900

    template <class T>
    inline void hash_combine(std::size_t & seed, const T & v)
    {
        using namespace std;
        hash<T> hasher;
        seed ^= hasher(v) + std::size_t(0x9e3779b9) + (seed << 6) + (seed >> 2);
    }

    template<typename S, typename T>
    struct pair_hash
    {
        inline std::size_t operator()(const std::pair<S,T> & v) const
        {
             std::size_t seed = 0;
             hash_combine(seed, v.first);
             hash_combine(seed, v.second);
             return seed;
        }
    };

    
} // namespace Tools
