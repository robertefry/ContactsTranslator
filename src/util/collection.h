
#pragma once

#include <initializer_list>

namespace util
{

    template <typename _Tp>
    auto multi_compare_or(const _Tp& item, const std::initializer_list<_Tp>& list)
        -> bool
    {
        for (const auto& elem : list) {
            if (item == elem) return true;
        }
        return false;
    }

    template <typename _Tp>
    auto multi_compare_and(const _Tp& item, const std::initializer_list<_Tp>& list)
        -> bool
    {
        size_t counter = 0;
        for (const auto& elem : list) {
            if (item == elem) counter += 1;
        }
        return counter == list.size();
    }

}
