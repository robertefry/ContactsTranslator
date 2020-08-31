
#include "CSV.h"
#include "util/string.h"

#include <sstream>
#include <algorithm>

/******************************************************************************/
/* CSVCell ********************************************************************/
fileio::CSVCell::CSVCell(const std::string& str)
    : m_String{str}
{
}
/******************************************************************************/

/******************************************************************************/
/* CSVRow *********************************************************************/
fileio::CSVRow::CSVRow(const std::initializer_list<CSVCell>& cells)
    : std::vector<CSVCell>{cells}
{
    for (size_t j = 0; j < size(); ++j) {
        at(j).m_Row = m_Row;
        at(j).m_Col = j;
    }
}

auto fileio::CSVRow::ncols() const -> size_t
{
    return empty() ? 0 : size();
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
fileio::CSVTable::CSVTable(const std::initializer_list<CSVRow>& rows)
    : std::vector<CSVRow>{rows}
{
    for (size_t i = 0; i < size(); ++i) {
        at(i).m_Row = i;
    }
}

auto fileio::CSVTable::nrows() const -> size_t
{
    return empty() ? 0 : size();
}

auto fileio::CSVTable::ncols() const -> size_t
{
    if (empty()) return 0;
    return std::max_element(begin(), end(),[](const auto& a, const auto& b) {
            return a.size() < b.size();
        })->size();
}

auto fileio::CSVTable::str() const -> std::string
{
    if (empty()) return "";

    // Calculate widths of each column
    std::vector<size_t> widths;
    widths.reserve(ncols());
    for (size_t j = 0; j < ncols(); ++j) {
        size_t max_width = at(0).at(j).str().size();
        for (size_t i = 1; i < nrows(); ++i) {
            max_width = std::max(max_width, at(i).at(j).str().size());
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
    for (size_t j = 1; j < ncols(); ++j) {
        ss << " ┊ " << util::pad_string(at(0).at(j).str(), widths.at(j));
    }
    ss << " │" << std::endl;
    // header divider
    ss << "│ " << util::repeat_string("┄", widths.at(0));
    for (size_t j = 1; j < ncols(); ++j) {
        ss << "┄┼┄" << util::repeat_string("┄", widths.at(j));
    }
    ss << " │" << std::endl;
    // csv entries
    for (size_t i = 1; i < nrows(); ++i) {
        ss << "│ " << util::pad_string(at(i).at(0).str(), widths.at(0));
        for (size_t j = 1; j < ncols(); ++j) {
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

        std::string str;
        std::stringstream line_stream {line};
        while (std::getline(line_stream, str, delim)) {
            row.push_back(CSVCell{str});
        }
        if (!line_stream && str.empty()) {
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
// /******************************************************************************/
