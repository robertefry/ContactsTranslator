
#pragma once

#include <istream>
#include <string>
#include <vector>

namespace fileio
{

class CSVCell : public std::string
{
public:
    CSVCell() = default;
    CSVCell(std::string&&);
    CSVCell(const char*);

    auto str() const -> const std::string&;
};

class CSVRow : public std::vector<CSVCell>
{
public:
    CSVRow() = default;
    CSVRow(std::initializer_list<CSVCell>&&);

    auto str() const -> std::string;
};

class CSVTable : public std::vector<CSVRow>
{
public:
    CSVTable() = default;
    CSVTable(std::initializer_list<CSVRow>&&);

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
