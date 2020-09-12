
#include "fileio/CSV.h"
#include "contacts/Contact.h"

#include <iostream>

int main()
{
    constexpr char csv[] =
        "First Name,Last Name,Email Address 2,Email Address 1\n"
        "Robert,Fry,robert.fry@email.com,robert@fry.xyz\n"
        "Andrew,Fry,andrew.fry@email.com,andrew@fry.xyz";

    auto table_in = fileio::CSVReader::ReadCSVTable(csv, ',');
    std::cout << table_in.str() << std::endl;

    auto address_book = AddressBook{table_in};
    std::cout << address_book.str() << std::endl;

    auto table_out = fileio::CSVTable{address_book.table_view()};
    std::string csv_converted;
    fileio::CSVWriter::WriteCSVTable(table_out, csv_converted);
    std::cout << csv_converted << std::endl;
}
