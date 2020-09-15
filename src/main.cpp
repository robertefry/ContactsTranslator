
#include "fileio/CSV.h"
#include "contacts/Contact.h"
#include "util/collection.h"

#include <cstring>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc != 3) return -1;

    std::string contacts_src = argv[1];
    std::string contacts_dst = argv[2];

    auto file_in = std::ifstream{contacts_src};
    auto table_in = fileio::CSVReader::ReadCSVTable(file_in, ',');
    std::cout << table_in.str() << std::endl;

    auto address_book = AddressBook{table_in};
    address_book.format_all();
    std::cout << address_book.str() << std::endl;

    auto file_out = std::ofstream{contacts_dst};
    auto table_out = fileio::CSVTable{address_book.table_view()};
    fileio::CSVWriter::WriteCSVTable(table_out, file_out);
    file_out.close();
}
