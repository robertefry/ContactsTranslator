
#include "Contact.h"
#include "util/collection.h"
#include "util/string.h"

#include <vector>
#include <map>
#include <regex>

ContactHeader::ContactHeader(const fileio::CSVRow& header)
{
    constexpr auto find_field_matches = [](const auto& cells, const auto& regex)
    {
        std::vector<fileio::CSVCell> field_matches;
        for (const auto& cell : cells) {
            if (std::regex_search(cell.str(), regex)) {
                field_matches.push_back(cell);
            }
        }
        return field_matches;
    };

    constexpr auto find_field_index = [](const auto& cells, const auto& regex) -> long
    {
        const fileio::CSVCell* bestmatch;
        for (const auto& cell : cells) {
            if (std::regex_search(cell.str(), regex)
                && (bestmatch == nullptr || bestmatch->str() < cell.str()) )
            {
                bestmatch = &cell;
            }
        }
        return bestmatch == nullptr ? -1 : bestmatch->col();
    };

    // find email addresses
    {
        auto email_address_list = find_field_matches(header,
            std::regex{"(e[_\\- ]?)?mail", std::regex::icase});

        if (email_address_list.size() >= 1) {
            const auto index1 = email_address_list[0].col();
            EmailAddress1Extractor = [&index1](const fileio::CSVRow& row) {
                return row[index1].str();
            };
        }
        if (email_address_list.size() >= 2) {
            const auto index2 = email_address_list[1].col();
            EmailAddress2Extractor = [&index2](const fileio::CSVRow& row) {
                return row[index2].str();
            };
        }
    }

    // find phone numbers
    {
        const auto index_mobile_number = find_field_index(header,
            std::regex{"mob(ile)?\\s?(phone)?\\s?n(o|um(ber)?)?", std::regex::icase} );
        MobilePhoneNumberExtractor = [&index_mobile_number](const fileio::CSVRow& row) {
            return row[index_mobile_number].str();
        };

        const auto index_home_number = find_field_index(header,
            std::regex{"home\\s?(phone)?\\s?n(o|um(ber)?)?", std::regex::icase} );
        HomePhoneNumberExtractor = [&index_home_number](const fileio::CSVRow& row) {
            return row[index_home_number].str();
        };

        const auto index_work_number = find_field_index(header,
            std::regex{"work\\s?(phone)?\\s?n(o|um(ber)?)?", std::regex::icase} );
        WorkPhoneNumberExtractor = [&index_work_number](const fileio::CSVRow& row) {
            return row[index_work_number].str();
        };
    }

    // find names
    {
        const auto index_firstname = find_field_index(header,
            std::regex{"f(i?r)?(st)?\\s[\\s\\w]*name", std::regex::icase} );
        FirstNameExtractor = [&index_firstname](const fileio::CSVRow& row) {
            return row[index_firstname].str();
        };

        const auto index_lastname = find_field_index(header,
            std::regex{"la?(st)?\\s[\\s\\w]*name", std::regex::icase} );
        LastNameExtractor = [&index_lastname](const fileio::CSVRow& row) {
            return row[index_lastname].str();
        };

        const auto index_displayname = find_field_index(header,
            std::regex{"display\\s[\\s\\w]*name", std::regex::icase} );
        DisplayNameExtractor = [&,this](const fileio::CSVRow& row) {
            if (index_displayname > -1) {
                return row[index_displayname].str();
            }
            { // try constructing from names
                std::ostringstream display_from_names;
                const std::string first_name = FirstNameExtractor(row);
                if (!std::regex_match(first_name, std::regex{"\\s*"})) {
                    display_from_names << first_name;
                }
                const std::string last_name = LastNameExtractor(row);
                if (!std::regex_match(last_name, std::regex{"\\s*"})) {
                    display_from_names << last_name;
                }
                if (display_from_names.tellp()) {
                    return display_from_names.str();
                }
            }
            { // try constructing from email addresses
                const std::string email_address1 = EmailAddress1Extractor(row);
                if (!std::regex_match(email_address1, std::regex{"\\s*"})) {
                    return email_address1;
                }
                const std::string email_address2 = EmailAddress2Extractor(row);
                if (!std::regex_match(email_address2, std::regex{"\\s*"})) {
                    return email_address2;
                }
            }
            { // try constructing from phone numbers
                const std::string mobile_phone_number = MobilePhoneNumberExtractor(row);
                if (!std::regex_match(mobile_phone_number, std::regex{"\\s*"})) {
                    return mobile_phone_number;
                }
                const std::string home_phone_number = HomePhoneNumberExtractor(row);
                if (!std::regex_match(home_phone_number, std::regex{"\\s*"})) {
                    return home_phone_number;
                }
                const std::string work_phone_number = WorkPhoneNumberExtractor(row);
                if (!std::regex_match(work_phone_number, std::regex{"\\s*"})) {
                    return work_phone_number;
                }
            }
            return std::string();
        };
    }
}

Contact::Contact(const fileio::CSVRow& entry, const ContactHeader& header)
    : FirstName{header.FirstNameExtractor(entry)}
    , LastName{header.LastNameExtractor(entry)}
    , DisplayName{header.DisplayNameExtractor(entry)}
    , EmailAddress1{header.EmailAddress1Extractor(entry)}
    , EmailAddress2{header.EmailAddress2Extractor(entry)}
    , MobilePhoneNumber{header.MobilePhoneNumberExtractor(entry)}
    , HomePhoneNumber{header.HomePhoneNumberExtractor(entry)}
    , WorkPhoneNumber{header.WorkPhoneNumberExtractor(entry)}
{
}

static void formatAsPhoneNumberInplace(std::string& phone_number)
{
    // remove all whitespace
    std::remove_if(phone_number.begin(), phone_number.end(), isspace);

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
    std::remove_if(EmailAddress1.begin(), EmailAddress1.end(), isspace);

    // email address 1
    std::remove_if(EmailAddress2.begin(), EmailAddress2.end(), isspace);

    // mobile phone number
    formatAsPhoneNumberInplace(MobilePhoneNumber);

    // home phone number
    formatAsPhoneNumberInplace(HomePhoneNumber);

    // work phone number
    formatAsPhoneNumberInplace(WorkPhoneNumber);
}
