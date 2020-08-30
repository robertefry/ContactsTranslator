
#pragma once

#include <string>

namespace util
{

    auto pad_string(const std::string&, size_t, char = ' ')
        -> std::string;
    auto pad_string_inplace(std::string&, size_t, char = ' ')
        -> std::string&;

}
