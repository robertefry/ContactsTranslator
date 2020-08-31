
#pragma once

#include <cstring>
#include <string>
#include <iostream>

namespace util
{

    template <typename _Tp>
    auto pad_string_inplace(std::basic_string<_Tp>& str, size_t size, _Tp pad = ' ')
        -> std::basic_string<_Tp>&
    {
        if (str.size() > size) return str;
        size_t pad_size = size - str.size();
        return str.append(pad_size, pad);
    }

    template <typename _Tp>
    auto pad_string(const std::basic_string<_Tp>& str, size_t size, _Tp pad = ' ')
        -> std::basic_string<_Tp>
    {
        if (str.size() > size) return str;
        size_t pad_size = size - str.size();
        return str + std::basic_string<_Tp>(pad_size, pad);
    }

    template <typename _Tp>
    auto pad_string(const _Tp* str, size_t size, _Tp pad = ' ')
        -> std::basic_string<_Tp>
    {
        auto result = std::basic_string<_Tp>(str);
        if (strlen(str) > size) result;
        size_t pad_size = size - strlen(str);
        return result += std::basic_string<_Tp>(pad_size, pad);
    }

    template <typename _Tp>
    auto repeat_string(const std::basic_string<_Tp>& str, size_t n)
        -> std::basic_string<_Tp>
    {
        std::basic_string<_Tp> result;
        result.reserve(str.size() * n);
        for (size_t i = 0; i < n; ++i) result.append(str);
        return result;
    }

    template <typename _Tp>
    auto repeat_string(const _Tp* str, size_t n)
        -> std::basic_string<_Tp>
    {
        std::basic_string<_Tp> result;
        result.reserve(strlen(str) * n + 1);
        for (size_t i = 0; i < n; ++i) result.append(str);
        return result;
    }

}
