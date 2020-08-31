
#include "Contact.h"
#include "util/collection.h"
#include "util/string.h"
#include "util/datetime.h"

#include <map>
#include <initializer_list>
#include <algorithm>
#include <regex>

ContactName::ContactName(const std::string& str)
    : contact_name{str}
{
    format();
}

auto ContactName::format() -> ContactName&
{
    util::format_as_1line_proper_noun_inplace(contact_name);
    return *this;
}

auto ContactName::str() const -> std::string
{
    return contact_name;
}

ContactDOB::ContactDOB(const std::string& day, const std::string& month, const std::string& year)
    : day{day}, month{month}, year(year)
{
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

auto ContactDOB::str() const -> std::string
{
    std::stringstream ss;
    ss << day << " " << month << " " << year;
    return ss.str();
}

EmailAddress::EmailAddress(const std::string& str)
    : email_address{str}
{
    format();
}

auto EmailAddress::format() -> EmailAddress&
{
    // remove all whitespace
    std::remove_if(email_address.begin(), email_address.end(), util::is_whitespace);
}

auto EmailAddress::str() const -> std::string
{
    return email_address;
}

PhoneNumber::PhoneNumber(const std::string& str)
    : phone_number{str}
{
    format();
}

auto PhoneNumber::format() -> PhoneNumber&
{
    // remove all whitespace
    std::remove_if(phone_number.begin(), phone_number.end(), util::is_whitespace);

    // place iterator at the end of the country calling code
    auto stritr = phone_number.begin();
    if (phone_number.at(0) == '+' && phone_number.size() >= 3)
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
        if (util::multi_compare_or(phone_number.at(2),
            long_callcode_map.at(phone_number.at(1)) ))
        {
            stritr += 4;
        } else {
            stritr += 3;
        }
    }

    // Insert spaces where formatting requires
    if (stritr != phone_number.begin()) {
        phone_number.insert(stritr, ' ');
    }
    if (stritr + 6 < phone_number.end()) {
        stritr += 6;
        phone_number.insert(stritr, ' ');
    }

    return *this;
}

auto PhoneNumber::str() const -> std::string
{
    return phone_number;
}

ContactAddress::ContactAddress(
    const std::string& address_1,
    const std::string& address_2,
    const std::string& city,
    const std::string& county,
    const std::string& country,
    const std::string& postcode
)
    : address_1{address_1}
    , address_2{address_2}
    , city{city}
    , county{county}
    , country{country}
    , postcode{postcode}
{
    format();
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

auto ContactAddress::str() const -> std::string
{
    std::stringstream ss;
    ss << address_1 << std::endl << address_2 << std::endl << city << std::endl
        << county << std::endl << county << std::endl << postcode;
    return ss.str();
}

ContactJob::ContactJob(
    const std::string& job_title,
    const std::string& department,
    const std::string& organisation
)
    : job_title{job_title}
    , department{department}
    , organisation{organisation}
{
    format();
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

auto ContactJob::str() const -> std::string
{
    std::stringstream ss;
    ss << job_title << ", " << department << ", " << organisation;
    return ss.str();
}

WebsiteAddress::WebsiteAddress(const std::string& address)
    : address{address}
{
    format();
}

auto WebsiteAddress::format() -> WebsiteAddress&
{
    util::trim_string_inplace(address);
    return *this;
}
