
#pragma once

#include <string>
#include <vector>

class ContactName : public std::string
{
public:
    ContactName() = default;
    ContactName(const char*);
    ContactName(std::string&&);

    auto format() -> ContactName&;
    auto str() const -> const std::string&;
};

struct ContactDOB
{
    std::string year;
    std::string month;
    std::string day;

    auto format() -> ContactDOB&;
};

class EmailAddress : public std::string
{
public:
    EmailAddress() = default;
    EmailAddress(const char*);
    EmailAddress(std::string&&);

    auto format() -> EmailAddress&;
    auto isValid() const -> bool;
    auto str() const -> const std::string&;
};

class PhoneNumber : public std::string
{
public:
    PhoneNumber() = default;
    PhoneNumber(const char*);
    PhoneNumber(std::string&&);

    auto format() -> PhoneNumber&;
    auto str() const -> const std::string&;
};

struct ContactAddress
{
    std::string address_1;
    std::string address_2;
    std::string city;
    std::string county;
    std::string country;
    std::string postcode;

    auto format() -> ContactAddress&;
};

struct ContactJob
{
    std::string job_title;
    std::string department;
    std::string organisation;

    auto format() -> ContactJob&;
};

class WebsiteAddress : public std::string
{
public:
    WebsiteAddress();
    WebsiteAddress(const char*);
    WebsiteAddress(std::string&&);

    auto format() -> WebsiteAddress&;
};

class Contact
{
public:
    auto populateFromCSVEntry() -> Contact&;

protected:
    ContactName m_FirstName{};
    ContactName m_LastName{};
    ContactName m_DisplayName{};
    ContactName m_NickName{};
    ContactDOB m_DateOfBirth{};
    std::vector<EmailAddress> m_EmailAddressList{};
    PhoneNumber m_WorkPhoneNumber{};
    PhoneNumber m_HomePhoneNumber{};
    PhoneNumber m_MobilePhoneNumber{};
    std::string m_FaxNumber{};
    std::string m_PagerNumber{};
    ContactAddress m_HomeAddress{};
    ContactAddress m_WorkAddress{};
    ContactJob m_WorkDetails{};
    std::vector<std::string> m_WebPages{};
    std::vector<std::string> m_CustomFields{};
    std::string m_Notes{};
};
