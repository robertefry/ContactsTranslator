
#include "Contact.h"
#include "util/string.h"

#include <vector>
#include <regex>

#include <iostream>

/******************************************************************************/
/* ContactFieldMap ************************************************************/
ContactCSVInputMap::ContactCSVInputMap(const fileio::CSVRow& header)
{
    constexpr auto find_field_matches = [](const auto& cells, const auto& regex)
    {
        std::vector<fileio::CSVCell> field_matches;
        for (const auto& cell : cells) {
            if (std::regex_search(cell.str(), regex)) {
                field_matches.push_back(cell);
            }
        }
        std::sort(field_matches.begin(), field_matches.end(),
            [](const auto& cell1, const auto& cell2) { return cell1.str() < cell2.str(); } );
        return field_matches;
    };

    constexpr auto find_field_bestmatch = [](const auto& cells, const auto& regex)
    {
        const fileio::CSVCell* bestmatch = nullptr;
        for (const auto& cell : cells) {
            if (std::regex_search(cell.str(), regex)
                && (bestmatch == nullptr || bestmatch->str() < cell.str()) )
            {
                bestmatch = &cell;
            }
        }
        return bestmatch;
    };

    constexpr auto print_cell_match = [](const auto& field, const auto& cell)
    {
        std::cout << "Matched \"" << cell.str() << "\" at index " << cell.col()
            << " as field for \"" << field << "\"" << std::endl;
    };

    // find email addresses
    {
        auto email_address_list = find_field_matches(header,
            std::regex{"(e[_\\- ]?)?mail", std::regex::icase});

        if (email_address_list.size() >= 1) {
            print_cell_match(MapEmailAddress1.FieldName, email_address_list[0]);
            const auto index = email_address_list[0].col();
            MapEmailAddress1.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }
        if (email_address_list.size() >= 2) {
            print_cell_match(MapEmailAddress2.FieldName, email_address_list[1]);
            const auto index = email_address_list[1].col();
            MapEmailAddress2.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }
    }

    // find phone numbers
    {
        const auto bestmatch_mobile_number = find_field_bestmatch(header,
            std::regex{"mob(ile)?\\s?(phone)?\\s?n(o|um(ber)?)?", std::regex::icase} );
        if (bestmatch_mobile_number) {
            print_cell_match(MapMobilePhoneNumber.FieldName, *bestmatch_mobile_number);
            const auto index = bestmatch_mobile_number->col();
            MapMobilePhoneNumber.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }

        const auto bestmatch_home_number = find_field_bestmatch(header,
            std::regex{"home\\s?(phone)?\\s?n(o|um(ber)?)?", std::regex::icase} );
        if (bestmatch_home_number) {
            print_cell_match(MapHomePhoneNumber.FieldName, *bestmatch_home_number);
            const auto index = bestmatch_home_number->col();
            MapHomePhoneNumber.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }

        const auto bestmatch_work_number = find_field_bestmatch(header,
            std::regex{"home\\s?(phone)?\\s?n(o|um(ber)?)?", std::regex::icase} );
        if (bestmatch_work_number) {
            print_cell_match(MapWorkPhoneNumber.FieldName, *bestmatch_work_number);
            const auto index = bestmatch_work_number->col();
            MapWorkPhoneNumber.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }
    }

    // find names
    {
        const auto bestmatch_firstname = find_field_bestmatch(header,
            std::regex{"f(i?r)?(st)?\\s[\\s\\w]*name", std::regex::icase} );
        if (bestmatch_firstname) {
            print_cell_match(MapFirstName.FieldName, *bestmatch_firstname);
            const auto index = bestmatch_firstname->col();
            MapFirstName.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }

        const auto bestmatch_lastname = find_field_bestmatch(header,
            std::regex{"la?(st)?\\s[\\s\\w]*name", std::regex::icase} );
        if (bestmatch_lastname) {
            print_cell_match(MapLastName.FieldName, *bestmatch_lastname);
            const auto index = bestmatch_lastname->col();
            MapLastName.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        }

        const auto bestmatch_displayname = find_field_bestmatch(header,
            std::regex{"display\\s[\\s\\w]*name", std::regex::icase} );
        if (bestmatch_displayname) {
            print_cell_match(MapDisplayName.FieldName, *bestmatch_displayname);
            const auto index = bestmatch_displayname->col();
            MapDisplayName.MappingFunction = [=](const auto& row) {
                return row[index].str();
            };
        } else {
            MapDisplayName.MappingFunction = [&](const auto& row) {
                { // try constructing from names
                    std::ostringstream display_from_names;
                    const std::string first_name = MapFirstName.MappingFunction(row);
                    if (!std::regex_match(first_name, std::regex{"\\s*"})) {
                        display_from_names << first_name << " ";
                    }
                    const std::string last_name = MapLastName.MappingFunction(row);
                    if (!std::regex_match(last_name, std::regex{"\\s*"})) {
                        display_from_names << last_name << " ";
                    }
                    if (display_from_names.tellp()) {
                        return display_from_names.str();
                    }
                }
                { // try constructing from email addresses
                    const std::string email_address1 = MapEmailAddress1.MappingFunction(row);
                    if (!std::regex_match(email_address1, std::regex{"\\s*"})) {
                        return email_address1;
                    }
                    const std::string email_address2 = MapEmailAddress2.MappingFunction(row);
                    if (!std::regex_match(email_address2, std::regex{"\\s*"})) {
                        return email_address2;
                    }
                }
                { // try constructing from phone numbers
                    const std::string mobile_phone_number = MapMobilePhoneNumber.MappingFunction(row);
                    if (!std::regex_match(mobile_phone_number, std::regex{"\\s*"})) {
                        return mobile_phone_number;
                    }
                    const std::string home_phone_number = MapHomePhoneNumber.MappingFunction(row);
                    if (!std::regex_match(home_phone_number, std::regex{"\\s*"})) {
                        return home_phone_number;
                    }
                    const std::string work_phone_number = MapWorkPhoneNumber.MappingFunction(row);
                    if (!std::regex_match(work_phone_number, std::regex{"\\s*"})) {
                        return work_phone_number;
                    }
                }
                // No match found, so return an empty string.
                return std::string();
            };
        }
    }
}
/******************************************************************************/

/******************************************************************************/
/* Contact ********************************************************************/
Contact::Contact(const fileio::CSVRow& entry, const ContactCSVInputMap& mapper)
    : FirstName{mapper.MapFirstName(entry)}
    , LastName{mapper.MapLastName(entry)}
    , DisplayName{mapper.MapDisplayName(entry)}
    , EmailAddress1{mapper.MapEmailAddress1(entry)}
    , EmailAddress2{mapper.MapEmailAddress2(entry)}
    , MobilePhoneNumber{mapper.MapMobilePhoneNumber(entry)}
    , HomePhoneNumber{mapper.MapHomePhoneNumber(entry)}
    , WorkPhoneNumber{mapper.MapWorkPhoneNumber(entry)}
{
}

void Contact::format()
{
    // first name
    util::format_as_1line_proper_noun_inplace(FirstName);

    // last name
    util::format_as_1line_proper_noun_inplace(LastName);

    // display name
    util::format_as_1line_proper_noun_inplace(DisplayName);

    // // email address 1
    std::remove_if(EmailAddress1.begin(), EmailAddress1.end(), isspace);

    // email address 1
    std::remove_if(EmailAddress2.begin(), EmailAddress2.end(), isspace);

    // mobile phone number
    util::format_as_phone_number_inplace(MobilePhoneNumber);

    // home phone number
    util::format_as_phone_number_inplace(HomePhoneNumber);

    // work phone number
    util::format_as_phone_number_inplace(WorkPhoneNumber);
}

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
/******************************************************************************/

/******************************************************************************/
/* AddressBook ****************************************************************/
AddressBook::AddressBook(const fileio::CSVTable& table)
    : FieldMapper{ContactCSVInputMap{*table.cbegin()}}
{
    auto itr = table.cbegin();
    if (itr != table.cend()) {
        for (++itr; itr != table.cend(); ++itr) {
            m_Contacts.insert(Contact{*itr, FieldMapper});
        }
    }
}

void AddressBook::format_all()
{
    ContactSet contacts;
    for (auto contact : m_Contacts) {
        contact.format();
        contacts.insert(contact);
    }
    m_Contacts.clear();
    for (auto& contact : contacts) {
        m_Contacts.insert(std::move(contact));
    }
}

auto AddressBook::table_view() const -> bits::TableView<const std::string>
{
    std::vector<std::vector<const std::string*>> string_table;
    string_table.reserve(m_Contacts.size()+1);
    {
        std::vector<const std::string*> string_entry;
        string_entry.reserve(8);
        string_entry.push_back(&FieldMapper.MapFirstName.FieldName);
        string_entry.push_back(&FieldMapper.MapLastName.FieldName);
        string_entry.push_back(&FieldMapper.MapDisplayName.FieldName);
        string_entry.push_back(&FieldMapper.MapEmailAddress1.FieldName);
        string_entry.push_back(&FieldMapper.MapEmailAddress2.FieldName);
        string_entry.push_back(&FieldMapper.MapMobilePhoneNumber.FieldName);
        string_entry.push_back(&FieldMapper.MapHomePhoneNumber.FieldName);
        string_entry.push_back(&FieldMapper.MapWorkPhoneNumber.FieldName);
        string_table.push_back(std::move(string_entry));
    }
    for (auto itr = m_Contacts.cbegin(); itr != m_Contacts.cend(); ++itr) {
        std::vector<const std::string*> string_entry;
        string_entry.reserve(8);
        string_entry.push_back(&itr->FirstName);
        string_entry.push_back(&itr->LastName);
        string_entry.push_back(&itr->DisplayName);
        string_entry.push_back(&itr->EmailAddress1);
        string_entry.push_back(&itr->EmailAddress2);
        string_entry.push_back(&itr->MobilePhoneNumber);
        string_entry.push_back(&itr->HomePhoneNumber);
        string_entry.push_back(&itr->WorkPhoneNumber);
        string_table.push_back(std::move(string_entry));
    }
    return bits::TableView<const std::string>{string_table};
}

auto AddressBook::str() const -> std::string
{
    if (m_Contacts.empty()) return "";

    const auto to_string = [](const auto& str) { return str; };
    return table_view().str(to_string);
}
/******************************************************************************/
