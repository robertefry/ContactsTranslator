
#pragma once

#include "fileio/CSV.h"

#include <string>
#include <vector>

class ContactName
{
public:
    explicit ContactName() = default;
    ContactName(const std::string&);

    auto format() -> ContactName&;
    auto str() const -> std::string;

public:
    std::string contact_name{};
};

class ContactDOB
{
public:
    explicit ContactDOB() = default;
    ContactDOB(const std::string& day, const std::string& month, const std::string& year);

    auto format() -> ContactDOB&;
    auto str() const -> std::string;

public:
    std::string year{};
    std::string month{};
    std::string day{};
};

class EmailAddress
{
public:
    EmailAddress() = default;
    EmailAddress(const std::string&);

    auto format() -> EmailAddress&;
    auto str() const -> std::string;

public:
    std::string email_address{};
};

class PhoneNumber
{
public:
    explicit PhoneNumber() = default;
    PhoneNumber(const std::string&);

    auto format() -> PhoneNumber&;
    auto str() const -> std::string;

public:
    std::string phone_number{};
};

class ContactAddress
{
public:
    explicit ContactAddress() = default;
    ContactAddress(const std::string& address_1, const std::string& address_2,
        const std::string& city, const std::string& county,
        const std::string& country, const std::string& postcode );

    auto format() -> ContactAddress&;
    auto str() const -> std::string;

public:
    std::string address_1;
    std::string address_2;
    std::string city;
    std::string county;
    std::string country;
    std::string postcode;
};

class ContactJob
{
public:
    explicit ContactJob() = default;
    ContactJob(const std::string& job_title,
        const std::string& department, const std::string& organisation);

    auto format() -> ContactJob&;
    auto str() const -> std::string;

public:
    std::string job_title;
    std::string department;
    std::string organisation;
};

class WebsiteAddress
{
public:
    explicit WebsiteAddress() = default;
    WebsiteAddress(const std::string&);

    auto format() -> WebsiteAddress&;
    auto str() const -> std::string;

public:
    std::string address;
};

class Contact
{
public:
    Contact() = default;
    // TODO Contact(const fileio::CSVRow&);

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
