
#include "string.h"

#include <sstream>

auto util::pad_string(const std::string& str, size_t size, char pad)
    -> std::string
{
    if (str.size() > size) return str;

    size_t pad_size = size - str.size();
    return str + std::string(pad_size, pad);
}

auto util::pad_string_inplace(std::string& str, size_t size, char pad)
    -> std::string&
{
    if (str.size() > size) return str;

    size_t pad_size = size - str.size();
    return str.append(pad_size, pad);
}

auto util::repeat_string(const std::string& str, size_t n) -> std::string
{
    std::stringstream ss;
    for (size_t i = 0; i < n; ++i) {
        ss << str;
    }
    return ss.str();
}
