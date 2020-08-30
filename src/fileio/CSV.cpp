
#include "CSV.h"
#include "util/string.h"

#include <sstream>
#include <iostream>
#include <numeric>

auto fileio::CSVCell::str() const -> std::string
{
    return *this;
}

auto fileio::CSVRow::str() const -> std::string
{
    if (empty()) return "";

    std::ostringstream ss {at(0).str(), std::ios_base::ate};
    for (size_t i = 1; i < size(); ++i) {
        ss << ", " << at(i).str();
    }
    return ss.str();
}

auto fileio::CSVTable::rows() const -> size_t
{
    return size();
}

auto fileio::CSVTable::cols() const -> size_t
{
    return empty() ? 0 : at(0).size();
}

auto fileio::CSVTable::str() const -> std::string
{
    if (empty()) return "";

    std::vector<size_t> widths;
    widths.reserve(cols());
    for (size_t j = 0; j < cols(); ++j) {
        size_t max_width = at(0).at(j).size();
        for (size_t i = 1; i < rows(); ++i) {
            max_width = std::max(max_width, at(i).at(j).size());
        }
        widths.push_back(max_width);
    }

    std::ostringstream ss;
    ss << util::pad_string(at(0).at(0).str(), widths.at(0));
    for (size_t j = 1; j < cols(); ++j) {
        ss << " | " << util::pad_string(at(0).at(j).str(), widths.at(j));
    }
    ss << std::endl << std::string(widths.at(0), '-');
    for (size_t j = 1; j < widths.size(); ++j) {
        ss << "-+-" << std::string(widths.at(j), '-');
    }
    for (size_t i = 1; i < rows(); ++i) {
        ss << std::endl << util::pad_string(at(i).at(0).str(), widths.at(0));
        for (size_t j = 1; j < cols(); ++j) {
            ss << " | " << util::pad_string(at(i).at(j).str(), widths.at(j));
        }
    }
    return ss.str();
}

auto fileio::CSVReader::ReadCSVTable(std::istream& istr, char delim)
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
            row.push_back(CSVCell{});
        }
        table.push_back(row);
    }
    return table;
}

auto fileio::CSVReader::ReadCSVTable(const std::string& str, char delim) -> CSVTable
{
    std::istringstream istr(str);
    return CSVReader::ReadCSVTable(istr, delim);
}
