
#pragma once

#include <functional>

namespace util
{

    inline size_t hash_combine(size_t& seed)
    {
        return seed;
    }

    template <typename _Tp, typename... _Targs>
    inline size_t hash_combine(size_t& seed, const _Tp& v, _Targs... rest)
    {
        std::hash<_Tp> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        return hash_combine(seed, rest...);
    }

    template <typename... _Targs>
    inline size_t hash_combine(size_t seed, _Targs... rest)
    {
        return hash_combine(seed, rest...);
    }

}
