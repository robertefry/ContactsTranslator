
#include "CSV.h"

#include <sstream>

auto fileio::CSVReader::ReadTable(std::istream& istr, char delim)
    -> fileio::CSVTable
{
    CSVTable table;
    while (!istr.eof())
    {
        CSVRow row;
        std::string line;
        std::getline(istr, line);

        CSVCell cell;
        std::stringstream line_stream {line};
        while (std::getline(line_stream, cell, delim)) {
            row.push_back(cell);
        }
        if (!line_stream && cell.empty()) {
            row.push_back("");
        }
        table.push_back(row);
    }
    return table;
}
