
#pragma once

#include <string>
#include <sstream>
#include <algorithm>

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
    auto repeat_string(const std::basic_string<_Tp>& str, size_t n)
        -> std::basic_string<_Tp>
    {
        std::basic_string<_Tp> result;
        result.reserve(str.size()*n);
        for (size_t i = 0; i < n; ++i) {
            result.append(str);
        }
        return result;
    }

    template <typename _Tp>
    auto trim_string_inplace(std::basic_string<_Tp>& str)
        -> std::basic_string<_Tp>&
    {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(),
            std::bind1st(std::not_equal_to<_Tp>(), ' ') ));
        str.erase(std::find_if(str.rbegin(), str.rend(),
            std::bind1st(std::not_equal_to<_Tp>(), ' ')).base(), str.end());
        return str;
    }

    template <typename _Tp>
    auto trim_string(const std::basic_string<_Tp>& str)
        -> std::basic_string<_Tp>
    {
        const auto first = std::find_if(str.begin(), str.end(),
            std::bind1st(std::not_equal_to<_Tp>(), ' ') );
        const auto last = std::find_if(str.rbegin(), str.rend(),
            std::bind1st(std::not_equal_to<_Tp>(), ' ')).base();
        return std::basic_string<_Tp>(first, last);
    }

    template <typename _Tp>
    auto format_as_proper_noun_inplace(std::basic_string<_Tp>& str)
        -> std::basic_string<_Tp>&
    {
        str[0] = toupper(str[0]);
        for (size_t i = 1; i < str.size(); ++i) {
            str[i] = is_whitespace(str[i-1]) ? toupper(str[i]) : tolower(str[i]);
        }
        return str;
    }

    template <typename _Tp>
    auto format_as_proper_noun(const std::basic_string<_Tp>& str)
        -> std::basic_string<_Tp>
    {
        std::basic_string<_Tp> result;
        result.reserve(str.size());
        result.push_back(toupper(str[0]));
        for (size_t i = 1; i < str.size(); ++i) {
            _Tp c = is_whitespace(str[i-1]) ? toupper(str[i]) : tolower(str[i]);
            result.push_back(c);
        }
        return result;
    }

    template <typename _Tp>
    auto format_as_1line_proper_noun_inplace(std::basic_string<_Tp>& str)
        -> std::basic_string<_Tp>&
    {
        std::replace_if(str.begin(), str.end(), util::is_whitespace, ' ');
        util::trim_string_inplace(str);
        str.erase(std::unique(str.begin(), str.end(),
            [](char a, char b){ return a == b == ' '; } ), str.end());
        util::format_as_proper_noun_inplace(str);
        return str;
    }

    template <typename _Tp>
    auto format_as_1line_proper_noun(const std::basic_string<_Tp>& str)
        -> std::basic_string<_Tp>
    {
        std::basic_string<_Tp> newstr {str};
        return format_as_1line_proper_noun_inplace(newstr);
    }

    bool is_whitespace(int c)
    {
        static constexpr std::initializer_list<int> whitespace
            = {' ','\t','\v','\f','\n','\r'};
        return std::find(whitespace.begin(), whitespace.end(), c) != whitespace.end();
    }

    template <typename _Tp>
    bool is_number(const std::basic_string<_Tp>& str)
    {
        return std::find_if_not(str.begin(), str.end(), isdigit) == str.end();
    }

}
