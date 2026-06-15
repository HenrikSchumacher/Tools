#pragma once

namespace Tools
{
    template<typename T> struct Hash
    {
        inline std::size_t operator()( cref<T> x ) const
        {
            return std::hash<T>()(x);
        }
    };
    
    template<IntQ T>
    struct Hash<T>
    {
        using is_avalanching = std::true_type; // instruct Boost.Unordered to not use post-mixing
        
        inline std::size_t operator()( cref<T> x_0 ) const
        {
            std::size_t x = static_cast<std::size_t>(x_0);
            x = (x ^ (x >> 30)) * std::size_t(0xbf58476d1ce4e5b9);
            x = (x ^ (x >> 27)) * std::size_t(0x94d049bb133111eb);
            x =  x ^ (x >> 31);
            return static_cast<std::size_t>(x);
        }
    };
    
//    // TODO: We need a better hash for strings.
//    template<> struct Hash<std::string_view>
//    {
//        inline std::size_t operator()( std::string_view s ) const
//        {
//            return std::hash<std::string_view>()(s);
//        }
//    };


//    // TODO: We need a better hash for strings.
//    template<> struct Hash<std::string>
//    {
//        inline std::size_t operator()( cref<std::string> s ) const
//        {
//            return std::hash<std::string>()(s);
//        }
//    };
    
    
    // https://stackoverflow.com/a/9729747/8248900
    
    template <class T, typename Hash_T = Hash<T>>
    inline void HashCombine(std::size_t & seed, const T & x)
    {
        Hash_T hasher;
        seed ^= hasher(x) + std::size_t(0x9e3779b9) + (seed << 6) + (seed >> 2);
    }
    
    template<typename S, typename T>
    struct Hash<std::pair<S,T>>
    {
        using is_avalanching [[maybe_unused]] = std::true_type; // instruct Boost.Unordered to not use post-mixing
        
        inline std::size_t operator()(const std::pair<S,T> & p) const
        {
            std::size_t seed = 0;
            HashCombine(seed, p.first);
            HashCombine(seed, p.second);
            return seed;
        }
    };
    
    
    template<typename T, std::size_t n>
    struct Hash<std::array<T,n>>
    {
        using is_avalanching [[maybe_unused]] = std::true_type; // instruct Boost.Unordered to not use post-mixing
        
        inline std::size_t operator()(const std::array<T,n> & a) const
        {
            std::size_t seed = 0;
            for( std::size_t i = 0; i < n; ++i )
            {
                HashCombine(seed, a[i]);
            }
            return seed;
        }
    };

    
    
//    // Obsolete; kept for backward compatibility.
//    template<typename S, typename T>
//    struct pair_hash
//    {
//        using is_avalanching [[maybe_unused]] = std::true_type; // instruct Boost.Unordered to not use post-mixing
//        
//        inline std::size_t operator()(const std::pair<S,T> & p) const
//        {
//            std::size_t seed = 0;
//            HashCombine(seed, p.first);
//            HashCombine(seed, p.second);
//            return seed;
//        }
//    };
    

//    // Obsolete; kept for backward compatibility.
//    struct array_hash
//    {
//        using is_avalanching [[maybe_unused]] = std::true_type; // instruct Boost.Unordered to not use post-mixing
//        
//        template<typename T, std::size_t n>
//        inline std::size_t operator()(const std::array<T,n> & a) const
//        {
//            std::size_t seed = 0;
//            for( std::size_t i = 0; i < n; ++i )
//            {
//                HashCombine(seed, a[i]);
//            }
//            return seed;
//        }
//    };

    
} // namespace Tools
