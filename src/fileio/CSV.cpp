
#include "CSV.h"
#include "util/string.h"

#include <sstream>
#include <iostream>
#include <numeric>

/******************************************************************************/
/* CSVCell ********************************************************************/
fileio::CSVCell::CSVCell(std::string&& str)
{
    this->assign(std::forward<std::string>(str));
}

fileio::CSVCell::CSVCell(const char* str)
{
    this->assign(str);
}

auto fileio::CSVCell::str() const -> const std::string&
{
    return *this;
}
/******************************************************************************/

/******************************************************************************/
/* CSVRow *********************************************************************/
fileio::CSVRow::CSVRow(std::initializer_list<CSVCell>&& cells)
{
    this->assign(std::forward<std::initializer_list<CSVCell>>(cells));
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
/******************************************************************************/

/******************************************************************************/
/* CSVTable *******************************************************************/
fileio::CSVTable::CSVTable(std::initializer_list<CSVRow>&& rows)
{
    this->assign(std::forward<std::initializer_list<CSVRow>>(rows));
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

    // Calculate widths of each column
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
    // top border
    ss << "╭─" << util::repeat_string("─", widths.at(0));
    for (size_t i = 1; i < widths.size(); ++i)
        ss << "───" << util::repeat_string("─", widths.at(i));
    ss << "─╮" << std::endl;
    // header items
    ss << "│ " << util::pad_string(at(0).at(0).str(), widths.at(0));
    for (size_t j = 1; j < cols(); ++j) {
        ss << " ┊ " << util::pad_string(at(0).at(j).str(), widths.at(j));
    }
    ss << " │" << std::endl;
    // header divider
    ss << "│ " << util::repeat_string("┄", widths.at(0));
    for (size_t j = 1; j < cols(); ++j) {
        ss << "┄┼┄" << util::repeat_string("┄", widths.at(j));
    }
    ss << " │" << std::endl;
    // csv entries
    for (size_t i = 1; i < rows(); ++i) {
        ss << "│ " << util::pad_string(at(i).at(0).str(), widths.at(0));
        for (size_t j = 1; j < cols(); ++j) {
            ss << " ┊ " << util::pad_string(at(i).at(j).str(), widths.at(j));
        }
        ss << " │" << std::endl;
    }
    // bottom border
    ss << "╰─" << util::repeat_string("─", widths.at(0));
    for (size_t i = 1; i < widths.size(); ++i)
        ss << "───" << util::repeat_string("─", widths.at(i));
    ss << "─╯";

    return ss.str();
}
/******************************************************************************/

/******************************************************************************/
/* CSVReader ******************************************************************/
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
/******************************************************************************/
