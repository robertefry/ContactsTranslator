
#include "Contact.h"
#include "util/collection.h"
#include "util/string.h"
#include "util/datetime.h"

#include <map>
#include <initializer_list>
#include <algorithm>

ContactName::ContactName(const char* str)
{
    this->assign(str);
    format();
}

ContactName::ContactName(std::string&& str)
{
    this->assign(str);
    format();
}

auto ContactName::format() -> ContactName&
{
    util::format_as_1line_proper_noun_inplace(*this);
    return *this;
}

auto ContactName::str() const -> const std::string&
{
    return *this;
}

auto ContactDOB::format() -> ContactDOB&
{
    // format the year
    std::remove_if(year.begin(), year.end(), util::is_whitespace);
    if (year.size() == 2) year.insert(0, "20");

    // format the month
    std::remove_if(month.begin(), month.end(), util::is_whitespace);
    if (util::is_number(month)) {
        month = util::number_to_date_string(std::stoi(month, nullptr, 10));
    }
    util::format_as_proper_noun_inplace(month);

    // format the day
    std::remove_if(day.begin(), day.end(), util::is_whitespace);
}

EmailAddress::EmailAddress(const char* str)
{
    this->assign(str);
    format();
}

EmailAddress::EmailAddress(std::string&& str)
{
    this->assign(std::forward<std::string>(str));
    format();
}

auto EmailAddress::format() -> EmailAddress&
{
    // remove all whitespace
    std::remove_if(this->begin(), this->end(), util::is_whitespace);
}

auto EmailAddress::isValid() const -> bool
{
    return true;
}

auto EmailAddress::str() const -> const std::string&
{
    return *this;
}

PhoneNumber::PhoneNumber(const char* str)
{
    this->assign(str);
    format();
}

PhoneNumber::PhoneNumber(std::string&& str)
{
    this->assign(std::forward<std::string>(str));
    format();
}

auto PhoneNumber::format() -> PhoneNumber&
{
    // remove all whitespace
    std::remove_if(this->begin(), this->end(), util::is_whitespace);

    // place iterator at the end of the country calling code
    auto stritr = this->begin();
    if (at(0) == '+' && size() >= 3)
    {
        const std::map<char,std::initializer_list<char>> long_callcode_map {
            {'0', {  } },
            {'1', {  } },
            {'2', { '1', '2', '3', '4', '5', '6', '9' } },
            {'3', { '5', '7', '8' } },
            {'4', { '2' } },
            {'5', { '0', '9' } },
            {'6', { '7', '8', '9' } },
            {'7', {  } },
            {'8', { '0', '5', '7', '8' } },
            {'9', { '6', '7', '9' } }
        };
        if (util::multi_compare_or(at(2), long_callcode_map.at(at(1)))) {
            stritr += 4;
        } else {
            stritr += 3;
        }
    }

    // Insert spaces where formatting requires
    if (stritr != this->begin()) {
        this->insert(stritr, ' ');
    }
    if (stritr + 6 < this->end()) {
        stritr += 6;
        this->insert(stritr, ' ');
    }

    return *this;
}

auto PhoneNumber::str() const -> const std::string&
{
    return *this;
}

auto ContactAddress::format() -> ContactAddress&
{
    // format address 1
    util::format_as_1line_proper_noun_inplace(address_1);

    // format address 2
    util::format_as_1line_proper_noun_inplace(address_2);

    // format city
    util::format_as_1line_proper_noun_inplace(city);

    // format county
    util::format_as_1line_proper_noun_inplace(county);

    // format country
    util::format_as_1line_proper_noun_inplace(country);

    // format postcode
    util::trim_string_inplace(postcode);
    for (auto& c : postcode) c = toupper(c);

    return *this;
}

auto ContactJob::format() -> ContactJob&
{
    // format job_title
    util::format_as_1line_proper_noun_inplace(job_title);

    // format department
    util::format_as_1line_proper_noun_inplace(department);

    // format organisation
    util::format_as_1line_proper_noun_inplace(organisation);

    return *this;
}

auto WebsiteAddress::format() -> WebsiteAddress&
{
    util::trim_string_inplace(*this);
    return *this;
}
