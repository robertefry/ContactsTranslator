
#pragma once

#include <array>
#include <string>

namespace util
{

    auto number_to_date_string(int num)
        -> std::string
    {
        static const std::array<std::string,12> months = {
            "January", "February", "March", "April", "May", "June", "July",
            "August", "September", "October", "November", "December" };
        return months[num%months.size()];
    }

}
