
#include "fileio/CSV.h"
#include "util/string.h"

#include <iostream>

int main()
{
    constexpr char csv[] =
        "First Name,Last Name,Email Address\n"
        "Robert,Fry,robert.fry@email.com\n"
        "Andrew,Fry,andrew.fry@email.com";

    auto table = fileio::CSVReader::ReadTable(csv, ',');
    std::cout << table.str() << std::endl;

}
