
#pragma once

#include "fileio/CSV.h"
#include "util/serial.h"

#include <string>
#include <unordered_set>

class ContactHeader
{
public:
    explicit ContactHeader() = default;
    ContactHeader(const fileio::CSVRow& header);

    size_t LocationOfFirstName{-1};
    size_t LocationOfLastName{-1};
    size_t LocationOfDisplayName{-1};
    size_t LocationOfEmailAddress1{-1};
    size_t LocationOfEmailAddress2{-1};
    size_t LocationOfWorkPhoneNumber{-1};
    size_t LocationOfHomePhoneNumber{-1};
    size_t LocationOfMobilePhoneNumber{-1};
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
    std::string WorkPhoneNumber{};
    std::string HomePhoneNumber{};
    std::string MobilePhoneNumber{};
};

class AddressBook
{
    using ContactSet = std::unordered_set<Contact,
        util::serial_hash<Contact>,util::serial_equal_to<Contact> >;

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
