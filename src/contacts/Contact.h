
#pragma once

#include "fileio/CSV.h"
#include "util/hash.h"

#include <string>
#include <unordered_set>
#include <functional>

class ContactHeader
{
    using ContactFieldExtractor = std::function<std::string(const fileio::CSVRow&)>;

public:
    explicit ContactHeader() = default;
    ContactHeader(const fileio::CSVRow& header);

    ContactFieldExtractor FirstNameExtractor{};
    ContactFieldExtractor LastNameExtractor{};
    ContactFieldExtractor DisplayNameExtractor{};
    ContactFieldExtractor EmailAddress1Extractor{};
    ContactFieldExtractor EmailAddress2Extractor{};
    ContactFieldExtractor MobilePhoneNumberExtractor{};
    ContactFieldExtractor HomePhoneNumberExtractor{};
    ContactFieldExtractor WorkPhoneNumberExtractor{};
};

class Contact
{
public:
    explicit Contact() = default;
    Contact(const fileio::CSVRow& entry, const ContactHeader& header);

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

bool operator==(const Contact& contact1, const Contact& contact2)
{
    if (contact1.FirstName != contact2.FirstName) return false;
    if (contact1.LastName != contact2.LastName) return false;
    if (contact1.DisplayName != contact2.DisplayName) return false;
    if (contact1.EmailAddress1 != contact2.EmailAddress1) return false;
    if (contact1.EmailAddress2 != contact2.EmailAddress2) return false;
    if (contact1.MobilePhoneNumber != contact2.MobilePhoneNumber) return false;
    if (contact1.HomePhoneNumber != contact2.HomePhoneNumber) return false;
    if (contact1.WorkPhoneNumber != contact2.WorkPhoneNumber) return false;
    return true;
}

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

class AddressBook
{
    using ContactSet = std::unordered_set<Contact>;

public:
    explicit AddressBook() = default;

    inline auto empty() { return m_Contacts.empty(); }
    inline auto size() { return m_Contacts.size(); }
    inline auto begin() { return m_Contacts.begin(); }
    inline auto end() { return m_Contacts.end(); }
    inline auto cbegin() { return m_Contacts.cbegin(); }
    inline auto cend() { return m_Contacts.cend(); }

    inline auto find(const Contact& contact) { return m_Contacts.find(contact); }
    inline auto find(const Contact& contact) const { return m_Contacts.find(contact); }

    inline void insert(const Contact& contact) { m_Contacts.insert(contact); }
    inline void insert(Contact&& contact) { m_Contacts.insert(std::forward<Contact>(contact)); }
    inline auto erase(const Contact&& contact) { return m_Contacts.erase(contact); }
    inline auto erase(ContactSet::const_iterator itr) { return m_Contacts.erase(itr); }
    inline auto erase(ContactSet::const_iterator first, ContactSet::const_iterator last)
        { return m_Contacts.erase(first, last); }
    inline void clear() { m_Contacts.clear(); }

protected:
    ContactSet m_Contacts{};
};
