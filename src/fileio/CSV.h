
#pragma once

#include <istream>
#include <string>
#include <vector>

namespace fileio
{

class CSVCell : public std::string
{
public:
    auto str() const -> std::string;
};

class CSVRow : public std::vector<CSVCell>
{
public:
    auto str() const -> std::string;
};

class CSVTable : public std::vector<CSVRow>
{
public:
    auto rows() const -> size_t;
    auto cols() const -> size_t;

    auto str() const -> std::string;
};

namespace CSVReader
{
    auto ReadCSVTable(std::istream&, char delim)
        -> CSVTable;
    auto ReadCSVTable(const std::string&, char delim)
        -> CSVTable;
}

} // namespace fileio
