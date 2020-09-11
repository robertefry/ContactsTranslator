
#include "fileio/CSV.h"
#include "contacts/Contact.h"

#include <iostream>

int main()
{
    constexpr char csv[] =
        "First Name,Last Name,Email Address 2,Email Address 1\n"
        "Robert,Fry,robert.fry@email.com,robert@fry.xyz\n"
        "Andrew,Fry,andrew.fry@email.com,andrew@fry.xyz";

    auto table = fileio::CSVReader::ReadCSVTable(csv, ',');
    std::cout << table.str() << std::endl;

    auto address_book = AddressBook{table};
    std::cout << address_book.str() << std::endl;
}
