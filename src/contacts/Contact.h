
#pragma once

#include "fileio/CSV.h"
#include "util/hash.h"

#include <unordered_set>
#include <functional>
#include <string>

class ContactCSVInputMap
{
public:

    using CSVMappingFunction = std::function<std::string(const fileio::CSVRow&)>;
    class CSVMapper
    {
    public:
        explicit CSVMapper(const std::string& name)
            : FieldName{name}
        {}
        auto operator()(const fileio::CSVRow& row) const -> std::string
        {
            return MappingFunction(row);
        }

        inline static const CSVMappingFunction DefaultCSVMappingFunction =
            [](const auto&) { return ""; };
        CSVMappingFunction MappingFunction{ DefaultCSVMappingFunction };
        std::string FieldName{};
    };

    explicit ContactCSVInputMap() = default;
    ContactCSVInputMap(const fileio::CSVRow& header);

    CSVMapper MapFirstName{"First Name"};
    CSVMapper MapLastName{"Last Name"};
    CSVMapper MapDisplayName{"Display Name"};
    CSVMapper MapEmailAddress1{"Email Address 1"};
    CSVMapper MapEmailAddress2{"Email Address 2"};
    CSVMapper MapMobilePhoneNumber{"Mobile Phone Number"};
    CSVMapper MapHomePhoneNumber{"Home Phone Number"};
    CSVMapper MapWorkPhoneNumber{"Work Phone Number"};
};

class Contact
{
public:
    explicit Contact() = default;
    Contact(const fileio::CSVRow& entry, const ContactCSVInputMap& mapper);

    void format();

    std::string FirstName{};
    std::string LastName{};
    std::string DisplayName{};
    std::string EmailAddress1{};
    std::string EmailAddress2{};
    std::string MobilePhoneNumber{};
    std::string HomePhoneNumber{};
    std::string WorkPhoneNumber{};

    friend bool operator==(const Contact& contact1, const Contact& contact2);
};

namespace std {
    template<>
    struct hash<Contact>
    {
        size_t operator()(const Contact& contact) const noexcept
        {
            return util::hash_combine(0,
                contact.FirstName, contact.LastName, contact.DisplayName,
                contact.EmailAddress1, contact.EmailAddress2,
                contact.MobilePhoneNumber, contact.HomePhoneNumber, contact.WorkPhoneNumber );
        }
    };
}

class AddressBook : public std::unordered_set<Contact>
{
public:
    explicit AddressBook() = default;
    AddressBook(const fileio::CSVTable& table);

    auto str() const -> std::string;

    const ContactCSVInputMap FieldMapper{};
};
