
#include "Contact.h"
#include "util/collection.h"
#include "util/string.h"

#include <map>

ContactHeader::ContactHeader(const fileio::CSVRow& header)
{
    // TODO: regex lookup for each field
}

Contact::Contact(const fileio::CSVRow& entry, const ContactHeader& header)
    : FirstName{entry[header.LocationOfFirstName]}
    , LastName{entry[header.LocationOfLastName]}
    , DisplayName{entry[header.LocationOfDisplayName]}
    , EmailAddress1{entry[header.LocationOfEmailAddress1]}
    , EmailAddress2{entry[header.LocationOfEmailAddress2]}
    , WorkPhoneNumber{entry[header.LocationOfWorkPhoneNumber]}
    , HomePhoneNumber{entry[header.LocationOfHomePhoneNumber]}
    , MobilePhoneNumber{entry[header.LocationOfMobilePhoneNumber]}
{
}

static void formatAsPhoneNumberInplace(std::string& phone_number)
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
}

void Contact::format()
{
    // first name
    util::format_as_1line_proper_noun_inplace(FirstName);

    // last name
    util::format_as_1line_proper_noun_inplace(LastName);

    // display name
    util::format_as_1line_proper_noun_inplace(DisplayName);

    // email address 1
    std::remove_if(EmailAddress1.begin(), EmailAddress1.end(), util::is_whitespace);

    // email address 1
    std::remove_if(EmailAddress2.begin(), EmailAddress2.end(), util::is_whitespace);

    // work phone number
    formatAsPhoneNumberInplace(WorkPhoneNumber);

    // home phone number
    formatAsPhoneNumberInplace(HomePhoneNumber);

    // mobile phone number
    formatAsPhoneNumberInplace(MobilePhoneNumber);
}
