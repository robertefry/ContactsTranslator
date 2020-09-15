
#include "CSV.h"
#include "util/string.h"

#include <iostream>
#include <sstream>
#include <algorithm>

/******************************************************************************/
/* CSVCell ********************************************************************/
fileio::CSVCell::CSVCell(const std::string& str, size_t row, size_t col)
    : m_String{str}
    , m_Row{row}
    , m_Col{col}
{
}
/******************************************************************************/

/******************************************************************************/
/* CSVRow *********************************************************************/
fileio::CSVRow::CSVRow(const std::vector<CSVCell>& cells, size_t row)
    : std::vector<CSVCell>{cells}
    , m_Row{row}
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
fileio::CSVTable::CSVTable(const std::vector<CSVRow>& rows)
    : std::vector<CSVRow>{rows}
{
    for (size_t i = 0; i < size(); ++i) {
        at(i).m_Row = i;
    }
}

fileio::CSVTable::CSVTable(const bits::TableView<const std::string>& tableview)
{
    reserve(tableview.nrows());
    for (size_t i = 0; i < tableview.nrows(); ++i) {
        CSVRow row{{}, i};
        row.reserve(tableview.ncols());
        for (size_t j = 0; j < tableview.ncols(); ++j) {
            row.push_back(CSVCell{*tableview.table()[i][j], i, j});
        }
        push_back(std::move(row));
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

auto fileio::CSVTable::table_view() const -> bits::TableView<const CSVCell>
{
    std::vector<std::vector<const CSVCell*>> table;
    table.resize(nrows());
    for (size_t i = 0; i < nrows(); ++i) {
        table[i].resize(ncols());
        for (size_t j = 0; j < ncols(); ++j) {
            table[i][j] = &(*this)[i][j];
        }
    }

    return bits::TableView<const CSVCell>{table};
}

auto fileio::CSVTable::str() const -> std::string
{
    if (empty()) return "";

    const auto to_str = [](const CSVCell& cell) { return cell.str(); };
    return table_view().str(to_str);
#include <sstream>
}
/******************************************************************************/

/******************************************************************************/
/* CSVReader ******************************************************************/
auto fileio::CSVReader::ReadCSVTable(std::istream& istr, char delim)
    -> fileio::CSVTable
{
    std::cout << "Trying to read from CSV table..." << std::endl;
    CSVTable table;

    std::string line;
    for (size_t numrow = 0; std::getline(istr, line); ++numrow)
    {
        std::cout << "  => Reading row " << numrow << std::endl;
        CSVRow row{{}, numrow};

        std::string str;
        std::stringstream line_stream{line};
        for (size_t numcol = 0; std::getline(line_stream, str, delim); ++numcol)
        {
            CSVCell cell{str, numrow, numcol};
            row.push_back(std::move(cell));
        }
        table.push_back(std::move(row));
    }
    std::cout << "Done!" << std::endl;
    return table;
}

auto fileio::CSVReader::ReadCSVTable(const std::string& str, char delim) -> CSVTable
{
    std::istringstream istr(str);
    return CSVReader::ReadCSVTable(istr, delim);
}
/******************************************************************************/

/******************************************************************************/
/* CSVWriter ******************************************************************/
void fileio::CSVWriter::WriteCSVTable(const fileio::CSVTable& table, std::ostream& ostr)
{
    std::string str;
    WriteCSVTable(table, str);
    ostr << str;
}

void fileio::CSVWriter::WriteCSVTable(const fileio::CSVTable& table, std::string& str)
{
    std::ostringstream ostr;
    for (size_t i = 0; i < table.nrows()-1; ++i) {
        for (size_t j = 0; j < table.ncols()-1; ++j) {
            ostr << table[i][j].str() << ',';
        }
        ostr << table[i][table.ncols()-1].str() << std::endl;
    }
    for (size_t j = 0; j < table.ncols()-1; ++j) {
        ostr << table[table.nrows()-1][j].str() << ',';
    }
    ostr << table[table.nrows()-1][table.ncols()-1].str();
    str = ostr.str();
}
/******************************************************************************/
