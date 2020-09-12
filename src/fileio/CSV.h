
#pragma once

#include "bits/table_view.h"

#include <istream>
#include <string>
#include <vector>
#include <initializer_list>

namespace fileio
{

class CSVCell;
class CSVRow;
class CSVTable;

class CSVCell
{
    friend CSVRow;

public:
    CSVCell(const std::string&, size_t row, size_t col);

    inline auto row() const { return m_Row; }
    inline auto col() const { return m_Col; }
    inline auto str() const -> const std::string& { return m_String; }

protected:
    std::string m_String{};
    size_t m_Row{};
    size_t m_Col{};
};

class CSVRow : public std::vector<CSVCell>
{
    friend CSVTable;

public:
    CSVRow(const std::vector<CSVCell>&, size_t row);

    inline auto row() const { return m_Row; }
    auto ncols() const -> size_t;

    auto str() const -> std::string;

protected:
    size_t m_Row{};
};

class CSVTable : public std::vector<CSVRow>
{
public:
    explicit CSVTable() = default;
    CSVTable(const std::vector<CSVRow>&);
    CSVTable(const bits::TableView<const std::string>&);

    auto nrows() const -> size_t;
    auto ncols() const -> size_t;

    auto table_view() const -> bits::TableView<const CSVCell>;
    auto str() const -> std::string;
};

namespace CSVReader
{
    auto ReadCSVTable(std::istream&, char delim)
        -> CSVTable;
    auto ReadCSVTable(const std::string&, char delim)
        -> CSVTable;
}

namespace CSVWriter
{
    // void WriteCSVTable(std::istream&, char delim);
    void WriteCSVTable(const fileio::CSVTable&, std::string&);
}

} // namespace fileio
